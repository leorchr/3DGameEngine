#pragma once
#include "BoneTransform.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

const size_t MAX_SKELETON_BONES = 96;

class Skeleton
{
public:
	struct Bone {
		BoneTransform localBindPose;
		string name;
		int parent;
	};

	size_t getNbBones() const { return bones.size(); }
	const Bone& getBone(size_t index) const { return bones[index]; }
	const vector<Bone>& getBones() const { return bones; }
	const vector<Matrix4>& getGlobalInvBindPoses() const { return globalInvBindPoses; }
	void setBones(const vector<Bone>& bonesP);
	void computeGlobalInvBindPose();
	void unload();
private:
	vector<Bone> bones;
	vector<Matrix4> globalInvBindPoses;
};