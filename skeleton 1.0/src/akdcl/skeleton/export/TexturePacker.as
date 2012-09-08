package akdcl.skeleton.export {
	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.IBitmapDrawable;
	import flash.display.MovieClip;
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	/**
	 * ...
	 * @author Akdcl
	 */
	final public class TexturePacker {
		private static function getNearest2N(_n:uint):uint{
			return _n & _n - 1 ? 1 << _n.toString(2).length : _n;
		}
		
		private var remainRectList:Array;
		private var textureList:Array;
		private var textureDic:Object;
		private var fitRectDic:Object;
		private var matrix:Matrix;
		private var rect:Rectangle;
		
		public function TexturePacker() {
			textureList = [];
			remainRectList = [];
			matrix = new Matrix();
			rect = new Rectangle();
			clear();
		}
		
		public function dispose():void {
			clear();
			textureList = null;
			remainRectList = null;
			matrix = null;
			rect = null;
		}
		
		//DisplayObject BitmapData
		public function addTexture(_texture:Object, _prefix:String = null, _id:String = null):void {
			if (!_id && ("name" in _texture)) {
				_id = _texture.name;
			}
			if (_id) {
				if (_prefix) {
					_id = _prefix + "_" + _id;
				}
			}else {
				return;
			}
			textureDic[_id] = _texture;
			textureList.push(_id);
		}
		
		public function addTexturesFromContainer(_container:DisplayObjectContainer, _prefix:String = null):void {
			var _len:uint = _container.numChildren;
			for (var _i:uint = 0; _i < _len; _i++) {
				addTexture(_container.getChildAt(_i), _prefix);
			}
		}
		
		public function packTextures(_widthMax:uint, _interval:uint = 2, _verticalSide:Boolean = false, _transparent:Boolean = true):TextureMix {
			if (textureList.length == 0) {
				return null;
			}
			//贴图按照大小排序
			textureList.sort(sortTexture);
			
			_widthMax = getNearest2N(Math.max(textureDic[textureList[0]].width, _widthMax));
			
			//预置一个较高的高度，可以考虑预算高度
			var _heightMax:uint = 20480;
			
			fitRectDic = { };
			remainRectList.length = 0;
			remainRectList.push(new Rectangle(0, 0, _widthMax, _heightMax));
			
			var _textureListCopy:Array = textureList.slice();
			var _rectID:int;
			var _texture:Object;
			var _textureID:String;
			var _isFit:Boolean;
			var _len:uint;
			var _width:uint;
			var _height:uint;
			var _strArr:Array;
			var _movieClipFrame:uint;
			var _movieClipID:String;
			
			var _rect:Rectangle;
			var _rectPrev:Rectangle;
			var _rectNext:Rectangle;
			
			do {
				//寻找最高的空白区域
				_rect = getHighestRect();
				_rectID = remainRectList.indexOf(_rect);
				_len = _textureListCopy.length;
				_isFit = false;
				for (var _i:uint = 0; _i < _len; _i++ ) {
					//逐个比较贴图对象是否适合该区域
					_textureID = _textureListCopy[_i];
					_texture = textureDic[_textureID];
					
					_rectPrev = _texture.getRect(_texture);
					
					_width = Math.ceil(_rectPrev.width) + _interval;
					_height = Math.ceil(_rectPrev.height) + _interval;
					if (_rect.width >= _width && _rect.height >= _height) {
						//考虑竖直贴图的合理摆放
						if (_verticalSide?(_rectPrev.height > _rectPrev.width*4?(_rectID > 0?(_rect.height - _height >= remainRectList[_rectID-1].height):true):true):true) {
							_isFit = true;
							break;
						}
					}
				}
				if (_isFit) {
					//如果合适，将贴图标记在此，并将矩形区域再次分区
					fitRectDic[_textureID] = new Rectangle(_rect.x, _rect.y, _width - _interval + 1, _height - _interval + 1);
					_textureListCopy.splice(_i, 1);
					remainRectList.splice(_rectID+1, 0, new Rectangle(_rect.x + _width, _rect.y, _rect.width - _width, _rect.height));
					_rect.y += _height;
					_rect.width = _width;
					_rect.height -= _height;
				}else {
					//不合适，则放弃这个矩形区域，把这个区域将与他相邻的矩形区域合并（与较高的一边合并）
					if (_rectID==0) {
						_rectNext = remainRectList[_rectID + 1];
					}else if (_rectID==remainRectList.length-1) {
						_rectNext = remainRectList[_rectID - 1];
					}else {
						_rectPrev = remainRectList[_rectID - 1];
						_rectNext = remainRectList[_rectID + 1];
						_rectNext = _rectPrev.height <= _rectNext.height?_rectNext:_rectPrev;
					}
					
					if (_rect.x < _rectNext.x) {
						_rectNext.x = _rect.x;
					}
					_rectNext.width = _rect.width + _rectNext.width;
					remainRectList.splice(_rectID, 1);
				}
				_len = _textureListCopy.length;
			}while (_len > 0);
			
			//整个贴图都已经放完了，压缩剩余高度，生成整个贴图
			_heightMax = getNearest2N(_heightMax - getLowestRect().height);
			var _bitmapData:BitmapData = new BitmapData(_widthMax, _heightMax, _transparent, 0xFF00FF);
			
			var _xml:XML =<TextureAtlas/>;
			var _textureXML:XML;
			_len = textureList.length;
			for (_i = 0; _i < _len; _i++) {
				_textureID = textureList[_i];
				_rectNext = fitRectDic[_textureID];
				_texture = textureDic[_textureID];
				if (_texture is DisplayObject) {
					_rect = (_texture as DisplayObject).getRect(_texture as DisplayObject);
					matrix.tx = _rectNext.x - _rect.x;
					matrix.ty = _rectNext.y - _rect.y;
				}else {
					//贴图是bitmapData的情况
					_rect = rect;
					_rect.x = 0;
					_rect.y = 0;
					_rect.width = _texture.width;
					_rect.height = _texture.height;
					matrix.tx = _rectNext.x;
					matrix.ty = _rectNext.y;
				}
				_bitmapData.draw(_texture as IBitmapDrawable, matrix, null, null, _rectNext);
				
				//生成对应的xml节点
				_textureXML =<SubTexture/>;
				_textureXML.@name = _textureID;
				_textureXML.@x = _rectNext.x;
				_textureXML.@y = _rectNext.y;
				_textureXML.@width = _rectNext.width;
				_textureXML.@height = _rectNext.height;
				
				_textureXML.@frameX = _rect.x;
				_textureXML.@frameY = _rect.y;
				
				_xml.appendChild(_textureXML);
			}
			
			fitRectDic = { };
			remainRectList.length = 0;
			
			return new TextureMix(_bitmapData, _xml);
		}
		
		public function clear():void {
			textureDic = { };
			textureList.length = 0;
		}
		
		private function getHighestRect():Rectangle {
			var _height:uint = 0;
			var _rectHighest:Rectangle;
			var _rect:Rectangle;
			var _len:uint = remainRectList.length;
			for (var _i:uint = 0; _i < _len; _i++ ) {
				_rect = remainRectList[_i];
				if (_rect.height > _height) {
					_height = _rect.height;
					_rectHighest = _rect;
				}
			}
			return _rectHighest;
		}
		
		private function getLowestRect():Rectangle {
			var _height:Number = Infinity;
			var _rectLowest:Rectangle;
			for each(var _rect:Rectangle in remainRectList) {
				if (_rect.height < _height) {
					_height = _rect.height;
					_rectLowest = _rect;
				}
			}
			return _rectLowest;
		}
		
		private function sortTexture(_id1:String, _id2:String):int {
			var _texture1:Object = textureDic[_id1];
			var _texture2:Object = textureDic[_id2];
			var _v1:uint=_texture1.width + _texture1.height;
			var _v2:uint=_texture2.width + _texture2.height;
			if (_v1 == _v2) {
				return _texture1.width > _texture2.width? -1: 1;
			}
			return _v1 > _v2? -1:1;
		}
	}
}