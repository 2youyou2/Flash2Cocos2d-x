package akdcl.skeleton.utils {
	import flash.utils.ByteArray;
	
	/**
	 * ...
	 * @author Akdcl
	 */
	
	public function extractXML(_byteArray:ByteArray):XML {
		if (_byteArray) {
			try {
				_byteArray.position = _byteArray.length - 4;
				var _strSize:int = _byteArray.readInt();
				var _position:uint = _byteArray.length - 4 - _strSize;
				var _xmlByte:ByteArray = new ByteArray();
				_xmlByte.writeBytes(_byteArray, _position, _strSize);
				_xmlByte.uncompress();
				_byteArray.length = _position;
				return XML(_xmlByte.readUTFBytes(_xmlByte.length));
			}catch (_e:Error) {
				throw "无效的骨骼数据！！！";
			}
		}
		return null;
	}
	
}