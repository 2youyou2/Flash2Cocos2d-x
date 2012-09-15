package akdcl.skeleton.utils {
	import flash.utils.ByteArray;
	
	import akdcl.skeleton.objects.SkeletonData;
	import akdcl.skeleton.objects.TextureData;
	
	/**
	 * ...
	 * @author Akdcl
	 */
	
	public function generateSkeletonData(_byteArray:ByteArray, _skeletonXML:XML = null):SkeletonData {
		if (_byteArray && !_skeletonXML) {
			_skeletonXML = extractXML(_byteArray);
		}
		if (_skeletonXML) {
			var _textureAtlasXML:XML = _skeletonXML.elements(ConstValues.TEXTURE_ATLAS)[0];
			delete _skeletonXML[ConstValues.TEXTURE_ATLAS];
			var _skeletonData:SkeletonData = new SkeletonData(_skeletonXML);
			_skeletonData.textureData = new TextureData(_textureAtlasXML, _byteArray);
			return _skeletonData;
		}
		return null;
	}
}