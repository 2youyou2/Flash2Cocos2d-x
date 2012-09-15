package akdcl.skeleton.utils {
	import akdcl.skeleton.objects.FrameData;
	
	/**
	 * ...
	 * @author Akdcl
	 */
	public function generateFrameData(_frameXML:XML):FrameData {
		var _frameData:FrameData = new FrameData(
			Number(_frameXML.attribute(ConstValues.A_X)),
			Number(_frameXML.attribute(ConstValues.A_Y)),
			Number(_frameXML.attribute(ConstValues.A_SKEW_X)) * ConstValues.ANGLE_TO_RADIAN,
			Number(_frameXML.attribute(ConstValues.A_SKEW_Y)) * ConstValues.ANGLE_TO_RADIAN,
			Number(_frameXML.attribute(ConstValues.A_SCALE_X)),
			Number(_frameXML.attribute(ConstValues.A_SCALE_Y)),
			int(_frameXML.attribute(ConstValues.A_DURATION)),
			Number(_frameXML.attribute(ConstValues.A_TWEEN_EASING))
		);
		_frameData.z = int(_frameXML.attribute(ConstValues.A_Z));
		_frameData.tweenRotate = int(_frameXML.attribute(ConstValues.A_TWEEN_ROTATE));
		
		_frameData.hide = Boolean(int(_frameXML.attribute(ConstValues.A_HIDE)));
		_frameData.image = String(_frameXML.attribute(ConstValues.A_IMAGE));
		_frameData.isArmature = Boolean(int(_frameXML.attribute(ConstValues.A_IS_ARMATURE)));
		_frameData.movement = String(_frameXML.attribute(ConstValues.A_MOVEMENT));
		
		_frameData.event = String(_frameXML.attribute(ConstValues.A_EVENT));
		_frameData.sound = String(_frameXML.attribute(ConstValues.A_SOUND));
		_frameData.soundEffect = String(_frameXML.attribute(ConstValues.A_SOUND_EFFECT));
		return _frameData;
	}
	
}