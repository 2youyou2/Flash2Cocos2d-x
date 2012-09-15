package akdcl.skeleton.export{
	import flash.display.BitmapData;
	
	/**
	 * 贴图类
	 * @author Akdcl
	 */
	final public class TextureMix{
		/**
		 * 贴图 BitmapData
		 */
		public var bitmapData:BitmapData;
		
		/**
		 * 贴图数据
		 */
		public var xml:XML;
		
		/**
		 * 贴图转换数据，比如 starling 转换成 Texture
		 */
		public var texture:Object;
		
		/**
		 * 构造函数
		 * @param _bmd 贴图 BitmapData
		 * @param _xml 贴图数据
		 */
		public function TextureMix(_bmd:BitmapData, _xml:XML = null):void {
			bitmapData = _bmd;
			xml = _xml;
		}
		
		/**
		 * 删除，释放
		 */
		public function dispose():void {
			bitmapData.dispose();
			xml = null;
			if (texture && ("dispose" in texture)) {
				texture.dispose();
			}
			texture = null;
		}
		
		/**
		 * 获得对应的贴图配置
		 * @param _id 贴图全称
		 * @return 返回贴图 XML 数据
		 */
		public function getTexture(_id:String):XML {
			return xml.children().(@name.toString() == _id)[0];
		}
		
		
		/**
		 * 获得同一前缀名的贴图数据列表
		 * @param _prefix 贴图前缀
		 * @return 返回贴图 XMLList 数据列表
		 */
		public function getTextureList(_prefix:String):XMLList {
			return xml.children().(@name.toString().indexOf(_prefix + "_") == 0);
		}
		
		/**
		 * 分析贴图全名中的前缀
		 * @param _fullName 贴图全称
		 * @return 返回贴图前缀
		 */
		public function getTexturePreFix(_fullName:String):String {
			var _arr:Array = _fullName.split("_");
			return _arr.length > 1?_arr[0]:null;
		}
		
		/**
		 * 分析贴图全名中的后缀
		 * @param _fullName 贴图全称
		 * @param _prefix 贴图前缀，未设置则自动获取
		 * @return 返回贴图后缀
		 */
		public function getTextureSuffix(_fullName:String, _prefix:String = null):String {
			if(!_prefix){
				_prefix = getTexturePreFix(_fullName);
			}
			if(_prefix){
				_prefix += "_";
				var _index:int = _fullName.indexOf(_prefix);
				if(_index == 0){
					return _fullName.substr(_index + _prefix.length);
				}
			}
			return null;
		}
	}
}