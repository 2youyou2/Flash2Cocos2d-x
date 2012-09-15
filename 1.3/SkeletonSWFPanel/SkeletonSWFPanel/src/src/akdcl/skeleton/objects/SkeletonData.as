package akdcl.skeleton.objects {
	import flash.utils.ByteArray;
	
	import akdcl.skeleton.utils.ConstValues;
	import akdcl.skeleton.utils.generateArmatureData;
	import akdcl.skeleton.utils.generateAnimationData;
	
	/**
	 * 
	 * @author Akdcl
	 */
	final public class SkeletonData extends BaseDicData {
		public var textureData:TextureData;
		
		private var animationDatas:Object;
		
		public function SkeletonData(_skeletonXML:XML) {
			super(null);
			animationDatas = { };
			if (_skeletonXML) {
				setData(_skeletonXML);
			}
		}
		
		public function getArmatureData(_name:String):ArmatureData {
			return datas[_name];
		}
		
		public function getAnimationData(_name:String):AnimationData {
			return animationDatas[_name];
		}
		
		public function setData(_skeletonXML:XML):void {
			name = _skeletonXML.attribute(ConstValues.A_NAME);
			
			for each(var _armatureXML:XML in _skeletonXML.elements(ConstValues.ARMATURES).elements(ConstValues.ARMATURE)) {
				addData(generateArmatureData(_armatureXML));
			}
			
			var _animationData:AnimationData;
			for each(var _animationXML:XML in _skeletonXML.elements(ConstValues.ANIMATIONS).elements(ConstValues.ANIMATION)) {
				_animationData = generateAnimationData(_animationXML);
				animationDatas[_animationData.name] = _animationData;
			}
		}
	}
}