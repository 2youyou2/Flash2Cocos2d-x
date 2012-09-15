package akdcl.skeleton.factorys {
	import akdcl.skeleton.Armature;
	import akdcl.skeleton.Bone;
	import akdcl.skeleton.display.PivotBitmap;
	import akdcl.skeleton.objects.AnimationData;
	import akdcl.skeleton.objects.ArmatureData;
	import akdcl.skeleton.objects.BoneData;
	import akdcl.skeleton.objects.FrameData;
	import akdcl.skeleton.objects.Node;
	import akdcl.skeleton.objects.SkeletonData;
	import akdcl.skeleton.objects.TextureData;
	import akdcl.skeleton.utils.ConstValues;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.EventDispatcher;
	import flash.geom.Matrix;
	import flash.geom.Rectangle;

	//import flash.utils.getDefinitionByName;
	
	/**
	 * 
	 * @author Akdcl
	 */
	public class BaseFactory extends EventDispatcher {
		private static const matrix:Matrix = new Matrix();
		
		private static var __lastInstance:BaseFactory;
		public static function get lastInstance():BaseFactory {
			if (!__lastInstance) {
				__lastInstance = new BaseFactory();
			}
			return __lastInstance;
		}
		
		public static function getTextureDisplay(_textureData:TextureData, _fullName:String):PivotBitmap {
			var _texture:XML = _textureData.getTextureXML(_fullName);
			if (_texture) {
				var _rect:Rectangle = new Rectangle(
					int(_texture.attribute(ConstValues.A_X)), 
					int(_texture.attribute(ConstValues.A_Y)), 
					int(_texture.attribute(ConstValues.A_WIDTH)), 
					int(_texture.attribute(ConstValues.A_HEIGHT))
				);
				var _img:PivotBitmap = new PivotBitmap(_textureData.bitmap.bitmapData);
				_img.scrollRect = _rect;
				_img.pivotX = int(_texture.attribute(ConstValues.A_PIVOT_X));
				_img.pivotY = int(_texture.attribute(ConstValues.A_PIVOT_Y));
				return _img;
			}
			return null;
		}
		
		private var __skeletonData:SkeletonData;
		public function get skeletonData():SkeletonData {
			return __skeletonData;
		}
		public function set skeletonData(_skeletonData:SkeletonData):void {
			__skeletonData = _skeletonData;
		}
		
		public function BaseFactory(_skeletonData:SkeletonData = null):void {
			super(this);
			skeletonData = _skeletonData;
		}
		
		public function buildArmature(_armatureName:String, _animationName:String = null):Armature {
			var _armatureData:ArmatureData = skeletonData.getArmatureData(_armatureName);
			if(!_armatureData){
				return null;
			}
			var _animationData:AnimationData = skeletonData.getAnimationData(_animationName || _armatureName);
			
			var _armature:Armature = generateArmature(_armatureName, _animationName);
			_armature.info.name = _armatureName;
			if (_armature) {
				_armature.animation.setData(_animationData);
				for each(var _boneName:String in _armatureData.getSearchList()) {
					generateBone(_armature, _armatureData, _boneName);
				}
				//_armature.updateBonesZ();
			}
			return _armature;
		}
		
		protected function generateArmature(_armatureName:String, _animationName:String = null):Armature {
			/*var _armatureDisplay:Object;
			var _className:String = _armatureName.split("/").join(".");
			var _class:Class = getDefinitionByName(_className) as Class;
			if (_class) {
				_armatureDisplay = new _class();
			}
			if (!_armatureDisplay) {
				return null;
			}*/
			return new Armature(new Sprite(), this);
		}
		
		protected function generateBone(_armature:Armature, _armatureData:ArmatureData, _boneName:String):void {
			var _boneData:BoneData = _armatureData.getData(_boneName);
			var _parentName:String = _boneData.parent;
			if (_parentName) {
				generateBone(_armature, _armatureData, _parentName);
			}
			var _bone:Bone = _armature.addBone(_boneName, _parentName, _boneData.z);
			var _imageName:String = _boneData.image;
			if (_imageName) {
				if (_boneData.isArmature) {
					var _armatureBone:Armature = buildArmature(_imageName);
					if (_armatureBone) {
						_bone.display = _armatureBone.display;
					}
				}else {
					_bone.display = generateBoneDisplay(_armature, _bone, _imageName);
				}
			}
			_bone.info.image = _imageName;
			_bone.setOriginPosition(_boneData.x, _boneData.y, _boneData.skewX, _boneData.skewY);
		}
		
		public function generateBoneDisplay(_armature:Armature, _bone:Bone, _imageName:String):Object {
			/*var _display:Object = _armature.display.getChildByName(_imageName);
			return _display;*/
			
			var _display:Object;
			var _clip:MovieClip = skeletonData.textureData.clip;
			if (_clip) {
				_clip.gotoAndStop(_clip.totalFrames);
				_clip.gotoAndStop(String(_imageName));
				if (_clip.numChildren > 0) {
					_display = _clip.getChildAt(0);
					if (!_display) {
						trace("无法获取影片剪辑，请确认骨骼 FLA 源文件导出player版本，与当前程序版本一致！");
					}
				}
			}else {
				skeletonData.textureData.updateBitmap();
				_display = getTextureDisplay(skeletonData.textureData, _imageName);
			}
			return _display;
		}
		
		public function updateDisplay(_display:Object, _node:Node):void {
			if (_display is PivotBitmap) {
				_display.tran_x = _node.x;
				_display.tran_y = _node.y;
				_display.tran_skewX = _node.skewX;
				_display.tran_skewY = _node.skewY;
				_display.tran_scaleX = _node.scaleX;
				_display.tran_scaleY = _node.scaleY;
				_display.update();
				return;
			}
			
			matrix.a = _node.scaleX * Math.cos(_node.skewY)
			matrix.b = _node.scaleX * Math.sin(_node.skewY)
			matrix.c = -_node.scaleY * Math.sin(_node.skewX);
			matrix.d = _node.scaleY * Math.cos(_node.skewX);
			matrix.tx = _node.x;
			matrix.ty = _node.y;
			_display.transform.matrix = matrix;	
		}
		
		public function displayContainer(_child:Object, _parent:Object = null, _zIndex:int = -1):Object {
			if (!_child) {
				return null;
			}
			if (_parent) {
				if(_zIndex < 0){
					_parent.addChild(_child);
				}else{
					_parent.addChildAt(_child, Math.min(_zIndex, _parent.numChildren));
				}
			}else if(_child.parent){
				_child.parent.removeChild(_child);
			}
			return _child;
		}
		
		//处理hide,image,armautre,event,sound
		public function boneKeyFrameRender(_bone:Bone, _keyFrame:FrameData):void {
			var _image:String = _bone.info.image;
			var _newImage:String = _keyFrame.image;
			if (_keyFrame.hide) {
				_bone.recycleDisplay();
			}else{
				if (!_bone.cycleDisplay(_newImage)) {
					if (_keyFrame.isArmature) {
						var _armatureBone:Armature = buildArmature(_newImage);
						if (_armatureBone) {
							_bone.display = _armatureBone.display;
						}
					}else {
						_bone.display = generateBoneDisplay(_bone.armature, _bone, _newImage);
					}
					_bone.info.image = _newImage;
					_bone.info.hide = false;
				}
				if(_keyFrame.z != _bone.info.z){
					_bone.info.z = _keyFrame.z;
					displayContainer(_bone.display, _bone.armature.display, _bone.info.z);
				}
			}
			
			if(_keyFrame.event && _bone.armature.boneEventCallback != null){
				_bone.armature.boneEventCallback(_keyFrame.event, _bone.info.name);
			}
			
			if(_keyFrame.sound && _bone.armature.soundEventCallback != null){
				_bone.armature.soundEventCallback(_keyFrame.sound, _keyFrame.soundEffect);
			}
		}
		
		public function animationEventHandler(_armature:Armature, _event:String, _movementID:String):void{
			if(_armature.armatureEventCallback != null){
				_armature.armatureEventCallback(_event, _movementID);
			}
		}
	}
}