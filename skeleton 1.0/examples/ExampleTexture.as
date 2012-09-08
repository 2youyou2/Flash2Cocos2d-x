package {
	import flash.display.Sprite;
	import flash.display.Bitmap;
	
	import akdcl.skeleton.export.TexturePacker;
	import akdcl.skeleton.export.TextureMix;
	
	/**
	 * ...
	 * @author Akdcl
	 */
    [SWF(width="800", height="600", nodeRate="30", backgroundColor="#999999")]
	public class ExampleTexture extends Sprite {
		public function ExampleTexture() {
			init();
		}
		
		private function init():void {
			//把stage上的显示对象拆分拼接成贴图，并生成对应的xml数据
			var _texturePacker:TexturePacker = new TexturePacker();
			
			var _textureContainer:Sprite;
			for (var _i:uint = 0; _i < numChildren; _i++ ) {
				_textureContainer = getChildAt(_i) as Sprite;
				//使用显示对象的名字当贴图的前缀
				_texturePacker.addTexturesFromContainer(_textureContainer,_textureContainer.name);
			}
			//生成贴图
			var _texture:TextureMix = _texturePacker.packTextures(512, 2, false, false);
			
			//数据清除
			_texturePacker.dispose();
			
			//看看贴图的样子
			addChild(new Bitmap(_texture.bitmapData));
			trace(_texture.xml);
		}
	}
}