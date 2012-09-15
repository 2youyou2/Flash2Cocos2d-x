package
{
	import adobe.utils.MMExecute;
	
	import mx.messaging.channels.StreamingAMFChannel;
	
	[Bindable]
	public class JSFL
	{
		public static const JSFL_URL:String = "SkeletonSWFPanel/skeleton.jsfl";
		
		public static var skeletonJSFL:String;
		
		public static function get isAvailable():Boolean{
			try{
				MMExecute("fl;");
				return true;
			}catch(_e:Error){
			}
			return false;
		}
		
		private static function runJSFL(_jsfl:String, _code:String):String{
			var _result:String;
			try {
				_result = MMExecute(_jsfl + _code);
			}catch(_e:Error){
			}
			return _result;
		}
		
		private static function xmlToString(_xml:XML):String{
			return <a a={_xml.toXMLString()}/>.@a.toXMLString();
		}
		
		public static function trace(...arg):String{
			var _str:String = "";
			for(var _i:uint = 0;_i < arg.length;_i ++){
				if(_i!=0){
					_str += ", ";
				}
				_str += arg[_i];
			}
			MMExecute("fl.trace('" +_str+ "');");
			return _str;
		}
		
		public static function getArmatureList(_isSelected:Boolean = false):Array{
			var _code:String = "String(getArmatureList(" + (_isSelected?"library.getSelectedItems()":"library.items") + "));";
			var _str:String = runJSFL(skeletonJSFL, _code);
			if(_str){
				var _arr:Array = _str.split(",");
			}
			return _arr;
		}
		
		public static function generateArmature(_name:String):XML{
			return XML(runJSFL(skeletonJSFL, "generateArmature('" + _name + "').toXMLString();"));
		}
		
		public static function clearTextureSWFItem(_length:uint):void{
			runJSFL(skeletonJSFL, "clearTextureSWFItem(" + _length + ");");
		}
		
		public static function addTextureToSWFItem(_itemName:String,  _instanceName:String, _index:uint):void{
			trace("addTextureToSWFItem(" + _itemName + ", "  + _instanceName + ", " + _index + ");");
			runJSFL(skeletonJSFL, "addTextureToSWFItem('" + _itemName + "', '"  + _instanceName + "', " + _index + ");");
		}
		
		public static function packTextures(_widthMax:uint, _interval:uint, _textureAtlasXML:XML = null):XML{
			var _str:String;
			if(_textureAtlasXML){
				_str = xmlToString(_textureAtlasXML);
				return XML(runJSFL(skeletonJSFL, "packTextures(" + _widthMax + "," + _interval + ",false,'" + _str + "').toXMLString();"));
			}
			runJSFL(skeletonJSFL, "packTextures(" + _widthMax + "," + _interval + ",false);");
			return null;
		}
		
		public static function exportSWF():String{
			return runJSFL(skeletonJSFL, "exportSWF();");
		}
		
		public static function changeArmatureConnection(_armatureName:String, _armatureXML:XML):void{
			runJSFL(skeletonJSFL, "changeArmatureConnection('" + _armatureName + "','" + xmlToString(_armatureXML) + "');");
		}
		
		public static function changeMovement(_armatureName:String, _movementName:String, _movementXML:XML):void{
			runJSFL(skeletonJSFL, "changeMovement('" + _armatureName + "','" + _movementName + "','" + xmlToString(_movementXML) + "');");
		}
	}
}