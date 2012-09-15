package akdcl.skeleton.utils {
	import akdcl.skeleton.objects.ArmatureData;
	import akdcl.skeleton.objects.BoneData;
	
	/**
	 * ...
	 * @author Akdcl
	 */
	public function generateArmatureData(_armatureXML:XML):ArmatureData {
		var _armatureData:ArmatureData = new ArmatureData(String(_armatureXML.attribute(ConstValues.A_NAME)));
		var _boneData:BoneData;
		
		for each(var _boneXML:XML in _armatureXML.elements(ConstValues.BONE)) {
			_boneData = new BoneData(
				Number(_boneXML.attribute(ConstValues.A_X)),
				Number(_boneXML.attribute(ConstValues.A_Y)),
				Number(_boneXML.attribute(ConstValues.A_SKEW_X)) * ConstValues.ANGLE_TO_RADIAN,
				Number(_boneXML.attribute(ConstValues.A_SKEW_Y)) * ConstValues.ANGLE_TO_RADIAN
			);
			_boneData.z = int(_boneXML.attribute(ConstValues.A_Z));
			_boneData.name = String(_boneXML.attribute(ConstValues.A_NAME));
			_boneData.parent = String(_boneXML.attribute(ConstValues.A_PARENT));
			_boneData.image = String(_boneXML.attribute(ConstValues.A_IMAGE));
			_boneData.isArmature = Boolean(int(_boneXML.attribute(ConstValues.A_IS_ARMATURE)));
			_armatureData.addData(_boneData, String(_boneXML.attribute(ConstValues.A_NAME)));
		}
		return _armatureData;
	}
	
}