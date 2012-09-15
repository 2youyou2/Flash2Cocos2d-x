package akdcl.skeleton.utils {
	import akdcl.skeleton.objects.AnimationData;
	
	/**
	 * ...
	 * @author Akdcl
	 */
	public function generateAnimationData(_animationXML:XML):AnimationData {
		var _animationData:AnimationData = new AnimationData(String(_animationXML.attribute(ConstValues.A_NAME)));
		for each(var _movementXML:XML in _animationXML.elements(ConstValues.MOVEMENT)) {
			_animationData.addData(generateMovementData(_movementXML));
		}
		return _animationData;
	}
	
}