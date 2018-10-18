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
	this->nodes = new BVHLinearNode[total_nodes];
	unsigned int offset = 0;
	Flatten(root, &offset);
}

BVH::~BVH()
{
	delete[] nodes;
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
			node->InitLeaf(first_shape_offset, num_shapes, bounds);
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
				node->InitLeaf(first_shape_offset, num_shapes, bounds);
				return node;
			}
			
			node->InitInterior(split_axis,
							   RecursiveBuild(shape_info, start, mid, total_nodes, ordered_shapes),
							   RecursiveBuild(shape_info, mid, end, total_nodes, ordered_shapes));
		}
		
	}
	
	return node;
}

unsigned int BVH::Flatten(BVHNode* node, unsigned int* offset)
{
	BVHLinearNode* linear_node = &nodes[*offset];
	linear_node->bb = node->bb;
	unsigned int my_offset = (*offset)++;
	if (node->num_shapes > 0) //Node is leaf
	{
		linear_node->shape_offset = node->first_shape_offset;
		linear_node->num_shapes = node->num_shapes;
	}
	else //Flatten
	{
		linear_node->split_axis = node->split_axis;
		linear_node->num_shapes = 0;
		Flatten(node->children[0], offset);
		linear_node->second_child_offset = Flatten(node->children[1], offset);
	}
	
	return my_offset;
}

bool BVH::Intersect(Ray* ray, SurfaceInteraction* isect) const
{
	bool hit = false;
	
	glm::vec3 inv_dir = 1.0f / ray->dir;
	//Since -z is inwards, this is flipped for dir_is_neg[2]
	int dir_is_neg[3] = { inv_dir.x < 0.0f, inv_dir.y < 0.0f, inv_dir.z >= 0.0f };
	
	unsigned int to_visit_offset = 0;
	unsigned int current_node_index = 0;
	unsigned int nodes_to_visit[64];
	while (true)
	{
		const BVHLinearNode* node = &nodes[current_node_index];
		if (node->bb.Intersect(*ray, inv_dir, dir_is_neg))
		{
			if (node->num_shapes > 0)
			{
				for (int i = 0; i < node->num_shapes; i++)
				{
					if (shapes[node->shape_offset + i]->Intersect(ray, isect, 0.00001f, 10000.0f))
					{
						hit = true;
					}
				}
				if (to_visit_offset == 0) { break; }
				current_node_index = nodes_to_visit[--to_visit_offset];
			}
			else
			{
				if (dir_is_neg[node->split_axis])
				{
					nodes_to_visit[to_visit_offset++] = current_node_index + 1;
					current_node_index = node->second_child_offset;
				}
				else
				{
					nodes_to_visit[to_visit_offset++] = node->second_child_offset;
					current_node_index++;
				}
			}
		}
		else
		{
			if (to_visit_offset == 0) { break; }
			current_node_index = nodes_to_visit[--to_visit_offset];
		}
	}
	
	return hit;
}























