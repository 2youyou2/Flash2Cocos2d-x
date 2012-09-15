package akdcl.skeleton.utils {
	import akdcl.skeleton.objects.MovementBoneData;
	
	/**
	 * ...
	 * @author Akdcl
	 */
	public function generateMovementBoneData(_boneXML:XML):MovementBoneData {
		var _movementBoneData:MovementBoneData = new MovementBoneData(
			String(_boneXML.attribute(ConstValues.A_NAME)),
			Number(_boneXML.attribute(ConstValues.A_MOVEMENT_SCALE)),
			Number(_boneXML.attribute(ConstValues.A_MOVEMENT_DELAY))
		);
		for each(var _frameXML:XML in _boneXML.elements(ConstValues.FRAME)) {
			_movementBoneData.addData(generateFrameData(_frameXML), _frameXML.childIndex());
		}
		return _movementBoneData;
	}
	
}