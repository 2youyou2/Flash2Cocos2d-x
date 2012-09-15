package akdcl.skeleton {
	import akdcl.skeleton.animation.Animation;
	import akdcl.skeleton.factorys.BaseFactory;
	import akdcl.skeleton.utils.skeletonNamespace;
	
	/**
	 * 
	 * @author Akdcl
	 */
	public class Armature extends Bone {
		public var animation:Animation;
		public var armatureEventCallback:Function;
		public var boneEventCallback:Function;
		public var soundEventCallback:Function;
		
		skeletonNamespace var bones:Object;
		
		override public function set display(_display:Object):void {}
		
		public function Armature(_display:Object, _factory:BaseFactory) {
			super(_factory);
			__display = _display;
			
			skeletonNamespace::bones = { };
			animation = new Animation(this);
		}
		
		override public function update():void{
			super.update();
			animation.update();
		}
		
		override public function dispose():void{
			super.dispose();
			animation = null;
			skeletonNamespace::bones = null;
			
			armatureEventCallback = null;
			boneEventCallback = null;
			soundEventCallback = null;
		}
			
		
		public function getBone(_name:String):Bone {
			return skeletonNamespace::bones[_name];
		}

		public function addBone(_name:String, _parentName:String = null, _z:int = -1):Bone {
			var _bone:Bone = skeletonNamespace::bones[_name];
			if (!_bone) {
				_bone = new Bone(factory);
				_bone.info.name = _name;
				skeletonNamespace::bones[_name] = _bone;
			}
			var _boneParent:Bone = skeletonNamespace::bones[_parentName];
			if (_boneParent) {
				_boneParent.addChild(_bone);
			}else {
				addChild(_bone);
			}
			if (_z < 0) {
				if (_bone.info.z < 0) {
					_bone.info.z = children.length - 1;
				}else {
					//_bone.z = _bone.z;
				}
			}else {
				_bone.info.z = _z;
			}
			return _bone;
		}
		
		public function removeBone(_name:String):void {
			var _bone:Bone = skeletonNamespace::bones[_name];
			if (_bone) {
				delete skeletonNamespace::bones[_name];
				removeChild(_bone);
			}
		}
		
		public function updateBonesZ():void {
			var _boneList:Array = [];
			var _bone:Bone;
			for each(_bone in skeletonNamespace::bones) {
				_boneList.push(_bone);
			}
			_boneList.sort(sortBoneZIndex);
			for each(_bone in _boneList) {
				if (_bone.display) {
					factory.displayContainer(_bone.display, __display);
				}
			}
		}
		
		private function sortBoneZIndex(_bone1:Bone, _bone2:Bone):int {
			return _bone1.info.z < _bone2.info.z?1: -1;
		}
	}
}