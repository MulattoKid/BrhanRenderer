#include <algorithm>
#include "BVH.h"
#include "Logger.h"

BVHShapeInfo::BVHShapeInfo(unsigned int shape_index, BoundingBox bb)
{
	this->shape_index = shape_index;
	this->bb = bb;
	this->centroid = 0.5f * bb.min + 0.5f * bb.max;
}

void BVHNode::InitLeaf(unsigned int first, unsigned int n, const BoundingBox& bb)
{
	first_shape_offset = first;
	num_shapes = n;
	this->bb = bb;
	children[0] = children[1] = NULL;
}

void BVHNode::InitInterior(SplitAxis axis, BVHNode* child0, BVHNode* child1)
{
	children[0] = child0;
	children[1] = child1;
	bb = BoundingBoxUnion(child0->bb, child1->bb);
	split_axis = axis;
	num_shapes = 0;
}

BVH::BVH() {}

BVH::BVH(const unsigned int max_shapes_in_node, const std::vector<Shape*>& shapes)
{
	if (shapes.size() == 0) { return; }
	this->max_shapes_in_node = max_shapes_in_node;
	this->shapes = shapes;
	
	//Initialize shape array
	std::vector<BVHShapeInfo> shape_info(shapes.size());
	for (size_t i = 0; i < shapes.size(); i++)
	{
		shape_info[i] = BVHShapeInfo(i, shapes[i]->bb);
	}
	
	//Build BVH
	unsigned int total_nodes = 0;
	std::vector<Shape*> ordered_shapes;
	BVHNode* root = RecursiveBuild(shape_info, 0, shapes.size(), &total_nodes, ordered_shapes);
	this->shapes = ordered_shapes;
	
	//Compute representation of depth-first traversal of BVH tree
	
}

struct BucketInfo
{
	int count = 0;
	BoundingBox bb;
};

BVHNode* BVH::RecursiveBuild(std::vector<BVHShapeInfo>& shape_info, unsigned int start, unsigned int end, unsigned int* total_nodes, std::vector<Shape*>& ordered_shapes)
{
	BVHNode* node = new BVHNode();
	(*total_nodes)++;
	
	//Compute bounding box of all primitives in node
	BoundingBox bounds;
	for (unsigned int i = start; i < end; i++)
	{
		bounds = BoundingBoxUnion(bounds, shape_info[i].bb);
	}
	
	unsigned int num_shapes = end - start;
	if (num_shapes <= max_shapes_in_node) //Create leaf node
	{
		size_t first_shape_offset = ordered_shapes.size();
		for (unsigned int i = start; i < end; i++)
		{
			unsigned int shape_index = shape_info[i].shape_index;
			ordered_shapes.push_back(shapes[shape_index]);
		}
		node->InitLeaf(first_shape_offset, num_shapes, bounds);
		return node;
	}
	else
	{
		//Compute bound of primitive centroids, choose split dimension
		BoundingBox centroid_bounds;
		for (unsigned int i = start; i < end; i++)
		{
			centroid_bounds = BoundingBoxUnion(centroid_bounds, shape_info[i].centroid);
		}
		SplitAxis split_axis = SplitAxis(centroid_bounds.MaximumExtent());
		unsigned int mid = (start + end) / 2;
		
		//All centroids are on the same point - unlikely to happen
		if (centroid_bounds.min[split_axis] == centroid_bounds.max[split_axis])
		{
			size_t first_shape_offset = ordered_shapes.size();
			for (unsigned int i = start; i < end; i++)
			{
				unsigned int shape_index = shape_info[i].shape_index;
				ordered_shapes.push_back(shapes[shape_index]);
			}
			node->InitLeaf(first_shape_offset, num_shapes, centroid_bounds);
			return node;
		}
		else //Partition shapes into two sets using SAH and build children
		{
			//Calculate bounds of buckets
			static const int num_buckets = 12;
			BucketInfo buckets[num_buckets];
			for (unsigned int i = start; i < end; i++)
			{
				int b = num_buckets * centroid_bounds.Offset(shape_info[i].centroid)[split_axis];
				if (b == num_buckets) { b -= 1; }
				buckets[b].count++;
				buckets[b].bb = BoundingBoxUnion(buckets[b].bb, shape_info[i].bb);
			}
			//Compute costs of splitting after each bucket
			float cost[num_buckets - 1];
			for (int i = 0; i < num_buckets - 1; i++)
			{
				BoundingBox b0, b1;
				int count0 = 0, count1 = 0;
				for (int j  = 0; j <= i; j++)
				{
					b0 = BoundingBoxUnion(b0, buckets[j].bb);
					count0 += buckets[j].count;
				}
				for (int j = i + 1; j < num_buckets; j++)
				{
					b1 = BoundingBoxUnion(b1, buckets[j].bb);
					count1 += buckets[j].count;
				}
				cost[i] = 0.125f + (count0 * b0.SurfaceArea() + count1 * b1.SurfaceArea()) / bounds.SurfaceArea();
			}
			//Find bucket with minimum cost
			float min_cost = cost[0];
			int min_cost_bucket = 0;
			for (int i = 1; i < num_buckets - 1; i++)
			{
				if (cost[i] < min_cost)
				{
					min_cost = cost[i];
					min_cost_bucket = i;
				}
			}
			//Split or create leaf depending on cost
			float leaf_cost = num_shapes;
			if (num_shapes > max_shapes_in_node || min_cost < leaf_cost)
			{
				BVHShapeInfo* p_mid = std::partition(&shape_info[start], &shape_info[end - 1] + 1,
						[=](const BVHShapeInfo& pi) {
							int b = num_buckets * centroid_bounds.Offset(pi.centroid)[split_axis];
							if (b == num_buckets) { b -= 1; }
							return b <= min_cost_bucket;
						});
				mid = p_mid - &shape_info[0];
			}
			else
			{
				size_t first_shape_offset = ordered_shapes.size();
				for (unsigned int i = start; i < end; i++)
				{
					unsigned int shape_index = shape_info[i].shape_index;
					ordered_shapes.push_back(shapes[shape_index]);
				}
				node->InitLeaf(first_shape_offset, num_shapes, centroid_bounds);
				return node;
			}
			
			node->InitInterior(split_axis,
							   RecursiveBuild(shape_info, start, mid, total_nodes, ordered_shapes),
							   RecursiveBuild(shape_info, mid, end, total_nodes, ordered_shapes));
		}
		
	}
	
	return node;
}
























