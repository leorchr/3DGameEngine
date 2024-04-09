#include "Skeleton.h"

void Skeleton::setBones(const vector<Bone>& bonesP) {
	bones = bonesP;
}

void Skeleton::computeGlobalInvBindPose() {
	globalInvBindPoses.resize(getNbBones());
	globalInvBindPoses[0] = bones[0].localBindPose.toMatrix();

	for (size_t i = 0; i < globalInvBindPoses.size(); i++)
	{
		Matrix4 localMat = bones[i].localBindPose.toMatrix();
		globalInvBindPoses[i] = localMat * globalInvBindPoses[bones[i].parent];
	}

	for (size_t i = 0; i < globalInvBindPoses.size(); i++)
	{
		globalInvBindPoses[i].invert();
	}
}

void Skeleton::unload() {

}