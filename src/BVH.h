#ifndef BVH_H
#define BVH_H

#include "Shape.h"

struct BVHShapeInfo
{
	unsigned int shape_index;
	BoundingBox bb;
	glm::vec3 centroid;
	
	BVHShapeInfo() {}
	BVHShapeInfo(unsigned int shape_index, 	BoundingBox bb);
};

enum SplitAxis { X = 0, Y = 1, Z = 2 };

struct BVHNode
{
	BoundingBox bb;
	BVHNode* children[2];
	SplitAxis split_axis;
	unsigned int first_shape_offset;
	unsigned int num_shapes;
	
	void InitLeaf(unsigned int first, unsigned int n, const BoundingBox& bb);
	void InitInterior(SplitAxis split_axis, BVHNode* child0, BVHNode* child1);
};

struct BVH
{
	unsigned int max_shapes_in_node;
	std::vector<Shape*> shapes;
	
	BVH();
	BVH(const unsigned int max_shapes_in_node, const std::vector<Shape*>& shapes);
	BVHNode* RecursiveBuild(std::vector<BVHShapeInfo>& shape_info, unsigned int start, unsigned int end, unsigned int* total_nodes, std::vector<Shape*>& ordered_shapes);
};

#endif
