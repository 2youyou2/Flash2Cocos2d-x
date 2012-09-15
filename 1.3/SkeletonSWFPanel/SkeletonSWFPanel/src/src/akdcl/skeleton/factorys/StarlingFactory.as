package akdcl.skeleton.factorys {
	import akdcl.skeleton.Armature;
	import akdcl.skeleton.Bone;
	import akdcl.skeleton.objects.Node;
	import akdcl.skeleton.objects.SkeletonData;
	import akdcl.skeleton.objects.TextureData;
	import akdcl.skeleton.utils.ConstValues;
	
	import flash.geom.Rectangle;
	
	import starling.display.Image;
	import starling.display.Sprite;
	import starling.textures.SubTexture;
	import starling.textures.Texture;
	
	/**
	 * 
	 * @author Akdcl
	 */
	public class StarlingFactory extends BaseFactory {
		
		private static var __lastInstance:StarlingFactory;
		public static function get lastInstance():StarlingFactory {
			if (!__lastInstance) {
				__lastInstance = new StarlingFactory();
			}
			return __lastInstance;
		}
		
		public static function getTextureDisplay(_textureData:TextureData, _fullName:String):Image {
			var _texture:XML = _textureData.getTextureXML(_fullName);
			if (_texture) {
				var _rect:Rectangle = new Rectangle(
					int(_texture.attribute(ConstValues.A_X)), 
					int(_texture.attribute(ConstValues.A_Y)), 
					int(_texture.attribute(ConstValues.A_WIDTH)), 
					int(_texture.attribute(ConstValues.A_HEIGHT))
				);
				var _img:Image = new Image(new SubTexture(_textureData.texture as Texture, _rect));
				_img.pivotX = int(_texture.attribute(ConstValues.A_PIVOT_X));
				_img.pivotY = int(_texture.attribute(ConstValues.A_PIVOT_Y));
				return _img;
			}
			return null;
		}
		
		override public function set skeletonData(_skeletonData:SkeletonData):void {
			super.skeletonData = _skeletonData;
			if (skeletonData) {
				skeletonData.textureData.updateBitmap();
			}
		}
		
		public function StarlingFactory(_skeletonData:SkeletonData = null):void {
			super(_skeletonData);
		}
		override protected function generateArmature(_armatureName:String, _animationName:String = null):Armature {
			if (!skeletonData.textureData.texture) {
				skeletonData.textureData.texture = Texture.fromBitmap(skeletonData.textureData.bitmap);
			}
			return new Armature(new Sprite(), this);
		}
		
		override public function generateBoneDisplay(_armature:Armature, _bone:Bone, _imageName:String):Object {
			return getTextureDisplay(skeletonData.textureData, _imageName);
		}
		
		override public function updateDisplay(_display:Object, _node:Node):void {
			_display.x = _node.x;
			_display.y = _node.y;
			_display.skewX = _node.skewX;
			_display.skewY = _node.skewY;
			_display.scaleX = _node.scaleX;
			_display.scaleY = _node.scaleY;
		}
	}
}