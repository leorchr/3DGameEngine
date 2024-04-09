#include "Animation.h"
#include "Skeleton.h"

void Animation::setNbBones(size_t nbBonesP) {
	nbBones = nbBonesP;
}

void Animation::setNbFrames(size_t nbFramesP) {
	nbFrames = nbFramesP;
}

void Animation::setDuration(float durationP) {
	duration = durationP;
}

void Animation::setFrameDuration(float frameDurationP) {
	frameDuration = frameDurationP;
}

void Animation::setTracks(const vector<vector<BoneTransform>>& tracksP) {
	tracks = tracksP;
}

void Animation::getGlobalPoseAtTime(vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const {
	if (outPoses.size() != nbBones) {
		outPoses.resize(nbBones);
	}

	size_t frame = static_cast<size_t>(inTime / frameDuration);
	size_t nextFrame = frame + 1;

	float pct = inTime / frameDuration - frame;

	if (tracks[0].size() > 0) {
		BoneTransform interp = BoneTransform::interpolate(tracks[0][frame], tracks[0][nextFrame], pct);
		outPoses[0] = interp.toMatrix();
	}
	else {
		outPoses[0] = Matrix4::identity;
	}
	const vector<Skeleton::Bone>& bones = inSkeleton->getBones();
	for (size_t bone = 1; bone < nbBones; bone++)
	{
		Matrix4 localMat;
		if (tracks[bone].size() > 0) {
			BoneTransform interp = BoneTransform::interpolate(tracks[bone][frame], tracks[bone][nextFrame], pct);
			localMat = interp.toMatrix();
		}
		outPoses[bone] = localMat * outPoses[bones[bone].parent];
	}
}

void Animation::unload() {

}