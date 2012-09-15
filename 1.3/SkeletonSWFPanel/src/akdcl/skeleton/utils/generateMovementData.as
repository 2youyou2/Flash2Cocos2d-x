package akdcl.skeleton.utils {
	import akdcl.skeleton.objects.MovementData;
	
	/**
	 * ...
	 * @author Akdcl
	 */
	public function generateMovementData(_movementXML:XML):MovementData {
		var _movementData:MovementData = new MovementData(
			String(_movementXML.attribute(ConstValues.A_NAME)),
			int(_movementXML.attribute(ConstValues.A_DURATION)),
			int(_movementXML.attribute(ConstValues.A_DURATION_TO)),
			int(_movementXML.attribute(ConstValues.A_DURATION_TWEEN)),
			Boolean(int(_movementXML.attribute(ConstValues.A_LOOP))),
			Number(_movementXML.attribute(ConstValues.A_TWEEN_EASING)[0])
		);
		for each(var _boneXML:XML in _movementXML.elements(ConstValues.BONE)) {
			_movementData.addData(generateMovementBoneData(_boneXML));
		}
		return _movementData;
	}
	
}