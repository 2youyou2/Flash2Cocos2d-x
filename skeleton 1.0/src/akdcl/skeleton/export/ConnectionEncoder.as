package akdcl.skeleton.export{
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.FrameLabel;
	import flash.display.MovieClip;

	import flash.geom.Point;

	/**
	 * ConnectionEncoder
	 * @author Akdcl
	 */
	final public class ConnectionEncoder {
		private static const NAN_VALUE:uint = 99;
		private static const AT:String = "@";

		private static const SKELETON:String = "skeleton";
		private static const BONE:String = "bone";
		private static const ANIMATION:String = "animation";
		private static const NAME:String = "name";
		private static const PARENT:String = "parent";
		private static const ROTATION:String = "rotation";
		private static const OFF_R:String = "offR";
		private static const X:String = "x";
		private static const Y:String = "y";
		private static const Z:String = "z";
		private static const WIDTH:String = "width";
		private static const HEIGHT:String = "height";
		private static const SCALE_X:String = "scaleX";
		private static const SCALE_Y:String = "scaleY";
		private static const ALPHA:String = "alpha";
		private static const DELAY:String = "delay";
		private static const SCALE:String = "scale";
		private static const FRAME:String = "frame";
		private static const EVENT_FRAME:String = "eventFrame";
		//使用这两个值的时候要注意   PACKER TEXTURE 时留的周围空白地区比较大的时候会影响效果
		private static const REGISTRATION_POINT_X = "registrationPointX"
		private static const REGISTRATION_POINT_Y = "registrationPointY"

		private static var pointTemp:Point = new Point();
		private static var frameNode:Object = {scaleX:1, scaleY:1, alpha:1, alpha:1, offR:0, totalFrames:1, x:0, y:0, rotation:0};

		public static function encode(_mc:MovieClip):XML {
			if (ContourInstaller.isInstalled(_mc)) {
				var _name:String = _mc.getName();
				if (_name) {
					var _xml:XML = <{SKELETON}/>;
					_xml[AT + NAME] = _name;
					formatXML(_mc.xml, _xml);
					generateBone(_mc, _xml);
					generateAnimation(_mc, _xml);
					return _xml;
				}
			}
			return null;
		}

		private static function formatXML(_xml:XML, _xmlCopy:XML = null, _level:uint = 0):void {
			var _boneCopy:XML;
			var _boneXMLList:XMLList = _xml.children();
			for each(var _boneXML:XML in _boneXMLList) {
				_boneCopy = _boneXML.copy();
				_boneCopy.setName(BONE);
				_boneCopy[AT + NAME] = _boneXML.name();
				if (_level > 0) {
					_boneCopy[AT + PARENT] = _boneXML.parent().name();
				}
				delete _boneCopy.*;
				_xmlCopy.appendChild(_boneCopy);
				if (_boneXML.children().length() > 0) {
					formatXML(_boneXML, _xmlCopy, _level+1);
				}
			}
		}

		private static function generateBone(_mc:MovieClip, _xml:XML):void {
			var _parent:DisplayObject;
			var _joint:DisplayObject;
			var _x:Number;
			var _y:Number;
			var _width:Number;
			var _height:Number;
			var _registrationPointX:Number;
			var _registrationPointY:Number;
			var _boneXML:XML;

			//按照深度顺序检索
			var _boneXMLList:XMLList = _xml.elements(BONE);
			var _length:uint = _mc.numChildren;
			for (var _i:uint = 0; _i < _length; _i++ ) {
				_joint = _mc.getChildAt(_i);
				_boneXML = _boneXMLList.(attribute(NAME) == _joint.name)[0];
				if (_boneXML) {
					_parent = _mc.getChildByName(_boneXML.attribute(PARENT));
					_registrationPointX = _joint.getBounds(_joint).x;
					_registrationPointY = _joint.getBounds(_joint).y;
					_width = _joint.getBounds(_joint).width;
					_height = _joint.getBounds(_joint).height;
					//trace(_joint.name + ":" + _registrationPointX + "  " +_registrationPointY);
					if (_parent) {
						transfromParentXY(pointTemp, _joint, _parent);
						_x = pointTemp.x;
						_y = pointTemp.y;
					}else {
						_x = _joint.x;
						_y = _joint.y;
					}
					_boneXML[AT + X] = Math.round(_x * 100) / 100;
					_boneXML[AT + Y] = Math.round(_y * 100) / 100;
					_boneXML[AT + Z] = _mc.getChildIndex(_joint);
					_boneXML[AT + WIDTH] = _width;
					_boneXML[AT + HEIGHT] = _height;
					_boneXML[AT + REGISTRATION_POINT_X] = _registrationPointX;
					_boneXML[AT + REGISTRATION_POINT_Y] = _registrationPointY;
				}else {
					trace("contour:" + _mc.getName(), "bone:" + _joint.name, "未找到对应的配置XML节点");
				}
			}
		}

		private static function generateAnimation(_mc:MovieClip, _xml:XML):void {
			var _frameLabel:FrameLabel;

			var _labelFrameLength:uint;
			var _name:String;
			var _joint:DisplayObject;

			var _animationXML:XML;
			var _boneXML:XML;
			var _boneNodeXML:XML;
			var _frameXMLList:XMLList;
			var _boneXMLList:XMLList = _xml.elements(BONE);

			var _currentLabels:Array = _mc.currentLabels;
			var _frameLabels:Array = formatFrameLabels(_currentLabels);
			var _length:uint = _frameLabels.length;
			for (var _i:uint = 0; _i < _length; _i++ ) {
				_frameLabel = _frameLabels[_i];
				//忽略第一帧的帧标签
				if (_frameLabel.frame == 1) {
					continue;
				}

				//获取带标签的帧的长度
				if (_i + 1 == _length) {
					_labelFrameLength = _mc.totalFrames - _frameLabel.frame + 1;
				}else {
					_labelFrameLength = _frameLabels[_i + 1].frame - _frameLabel.frame;
				}
				_animationXML =<{ANIMATION}/>;
				_animationXML[AT + NAME] = _frameLabel.name;
				_animationXML[AT + FRAME] = _labelFrameLength;
				_xml.appendChild(_animationXML);

				_mc.gotoAndStop(_frameLabel.name);
				for (var _k:uint = 0; _k < _labelFrameLength; _k++ ) {
					if (_k == _labelFrameLength - 1) {
						//为最后一个子标签帧修正其长度
						setFrameLabels(_animationXML, null, _mc.currentFrame + 1);
					}else if (_mc.currentFrameLabel && _mc.currentFrameLabel != _frameLabel.name) {
						//如果这帧带有子标签（不能是最后一帧和第一帧）
						setFrameLabels(_animationXML, _mc.currentFrameLabel.split("_").pop(), _mc.currentFrame);
					}

					for (var _j:uint = 0; _j < _mc.numChildren; _j++ ) {
						_joint = _mc.getChildAt(_j);
						_name = _joint.name;
						_boneXML = _boneXMLList.(attribute(NAME) == _name)[0];
						if (!_boneXML) {
							//没有配置xml的元件忽略
							continue;
						}

						setFrameNode(
							(_mc.getChildByName(_boneXML.attribute(PARENT))) as DisplayObjectContainer,
							_joint,
							_boneXML,
							_mc.getValue(_name, OFF_R)
						);

						_boneXML = null;
						//找到该关键的动画列表
						_frameXMLList = _animationXML.elements(_name);
						if (_frameXMLList.length() > 0) {
							//如果已经创建了该关键的动画列表则找到列表的最后一个
							_boneXML = _frameXMLList[_frameXMLList.length() - 1];
						}

						if (_boneXML && sameFrameNode(_boneXML)) {
							//忽略相同的节点
							if (_boneXML.attribute(FRAME).length() > 0) {
								_boneXML[AT + FRAME] = int(_boneXML.attribute(FRAME)) + 1;
							}else {
								//1+1
								_boneXML[AT + FRAME] = 2;
							}
						}else {
							addFrameNode(
								_name,
								_animationXML,
								_boneXML,
								_mc.getValue(_name, SCALE),
								_mc.getValue(_name, DELAY)
							);
						}
					}
					_mc.clearValues();
					_mc.nextFrame();
				}
			}
		}

		private static function setFrameLabels(_animationXML:XML, _labelName:String, _frame:int):void {
			if (_labelName) {
				var _node:XML =<{EVENT_FRAME}/>;
				_node[AT + NAME] = _labelName;
				_node[AT + FRAME] = _frame;
			}

			var _list:XMLList = _animationXML.elements(EVENT_FRAME);
			if (_list.length() > 0) {
				var _prevNode:XML = _list[_list.length() - 1];
				//为前一个子标签帧修正长度
				_prevNode[AT + FRAME] = _frame-int(_prevNode.attribute(FRAME));
				if (_node) {
					_animationXML.insertChildAfter(_prevNode, _node);
				}
			}else {
				if (_node) {
					_animationXML.prependChild(_node);
				}
			}
		}

		private static function formatFrameLabels(_frameLabels:Array):Array {
			var _labelsFormated:Array = [];
			var _length:uint = _frameLabels.length;
			var _frameLabel:FrameLabel;
			var _prevLabel:FrameLabel;
			for (var _i:uint = 0; _i < _length; _i++ ) {
				_frameLabel = _frameLabels[_i];
				//忽略第一帧的帧标签
				if (_frameLabel.frame == 1) {
					continue;
				}
				//如果标签是前一个标签的子节点，则忽略
				if (_prevLabel && _frameLabel.name.indexOf(_prevLabel.name + "_") == 0) {
					continue;
				}
				_labelsFormated[_labelsFormated.length] = _frameLabel;
				_prevLabel = _frameLabel;
			}
			return _labelsFormated;
		}

		private static function setFrameNode(_parent:DisplayObjectContainer, _joint:DisplayObject, _boneXML:XML, _offR:Number):void {
			if (_parent) {
				transfromParentXY(pointTemp, _joint, _parent, Number(_boneXML.attribute(X)), Number(_boneXML.attribute(Y)));
				frameNode.rotation = _joint.rotation - _parent.rotation;
				frameNode.x = pointTemp.x;
				frameNode.y = pointTemp.y;
			}else {
				frameNode.rotation = _joint.rotation;
				frameNode.x = _joint.x;
				frameNode.y = _joint.y;
			}

			frameNode.scaleX = _joint.scaleX;
			frameNode.scaleY = _joint.scaleY;
			frameNode.alpha = _joint.alpha;
			frameNode.offR = _offR;
			//
			formatFrameNode();
		}

		private static function addFrameNode(_name:String, _parentXML:XML, _prevNode:XML, _scale:Number, _delay:Number):void {
			var _frameNodeXML:XML =<{_name}/>;
			if (_prevNode) {
				_parentXML.insertChildAfter(_prevNode, _frameNodeXML);
			}else {
				_parentXML.appendChild(_frameNodeXML);
			}

			_frameNodeXML[AT + X] = frameNode.x;
			_frameNodeXML[AT + Y] = frameNode.y;
			_frameNodeXML[AT + ROTATION] = frameNode.rotation;
			if (frameNode.scaleX != NAN_VALUE) {
				_frameNodeXML[AT + SCALE_X] = frameNode.scaleX;
			}
			if (frameNode.scaleY != NAN_VALUE) {
				_frameNodeXML[AT + SCALE_Y] = frameNode.scaleY;
			}
			if (frameNode.alpha != NAN_VALUE) {
				_frameNodeXML[AT + ALPHA] = frameNode.alpha;
			}
			if (frameNode.offR) {
				_frameNodeXML[AT + OFF_R] = frameNode.offR;
			}
			if (_scale) {
				_frameNodeXML[AT + SCALE] = _scale;
			}
			if (_delay) {
				_delay %= 1;
				_frameNodeXML[AT + DELAY] = _delay;
			}
		}

		private static function formatFrameNode():void {
			frameNode.rotation = Math.round(frameNode.rotation * 100) / 100;
			frameNode.x = Math.round(frameNode.x * 100) / 100;
			frameNode.y = Math.round(frameNode.y * 100) / 100;
			frameNode.scaleX = Math.round(frameNode.scaleX * 20) / 20;
			frameNode.scaleY = Math.round(frameNode.scaleY * 20) / 20;
			frameNode.alpha = Math.round(frameNode.alpha * 20) / 20;

			if (Math.abs(frameNode.rotation) < 1) {
				frameNode.rotation = 0;
			}
			if (Math.abs(frameNode.x) < 1) {
				frameNode.x = 0;
			}
			if (Math.abs(frameNode.y) < 1) {
				frameNode.y = 0;
			}
			//如果scaleXY和alpha为1则忽略
			//node中默认为1
			if (frameNode.scaleX == 1) {
				frameNode.scaleX = NAN_VALUE;
			}else if (frameNode.scaleX > 3)  {
				//避免使用matrix使用大于3以上的值充当负值
				frameNode.scaleX = 3 - frameNode.scaleX;
			}
			if (frameNode.scaleY == 1) {
				frameNode.scaleY = NAN_VALUE;
			}else if (frameNode.scaleY > 3)  {
				//避免使用matrix使用大于3以上的值充当负值
				frameNode.scaleY = 3 - frameNode.scaleY;
			}
			if (frameNode.alpha == 1) {
				frameNode.alpha = NAN_VALUE;
			}
		}

		private static function sameFrameNode(_frameNodeXML:XML):Boolean {
			var _isSame:Boolean = true;
			//忽略相差一像素以内的位移和旋转
			_isSame = _isSame && int(_frameNodeXML.attribute(X)) == int(frameNode.x);
			_isSame = _isSame && int(_frameNodeXML.attribute(Y)) == int(frameNode.y);
			_isSame = _isSame && int(_frameNodeXML.attribute(ROTATION)) == int(frameNode.rotation);
			//scaleXY和alpha没有则默认为1
			_isSame = _isSame && (_frameNodeXML.attribute(SCALE_X).length() == 0?(frameNode.scaleX == NAN_VALUE):(Number(_frameNodeXML.attribute(SCALE_X)) == frameNode.scaleX));
			_isSame = _isSame && (_frameNodeXML.attribute(SCALE_Y).length() == 0?(frameNode.scaleY == NAN_VALUE):(Number(_frameNodeXML.attribute(SCALE_Y)) == frameNode.scaleY));
			_isSame = _isSame && (_frameNodeXML.attribute(ALPHA).length() == 0?(frameNode.alpha == NAN_VALUE):(Number(_frameNodeXML.attribute(ALPHA)) == frameNode.alpha));
			//offR没有则默认为0
			_isSame = _isSame && (_frameNodeXML.attribute(OFF_R).length() == 0?(frameNode.offR == 0):(Number(_frameNodeXML.attribute(OFF_R)) == frameNode.offR));
			return _isSame;
		}

		private static function transfromParentXY(_point:Point, _joint:DisplayObject, _parent:DisplayObject, _offX:Number = 0, _offY:Number = 0):void {
			var _dX:Number = _joint.x - _parent.x;
			var _dY:Number = _joint.y - _parent.y;
			var _r:Number = Math.atan2(_dY, _dX) - _parent.rotation * Math.PI / 180;
			var _len:Number = Math.sqrt(_dX * _dX + _dY * _dY);
			_point.x = _len * Math.cos(_r) - _offX;
			_point.y = _len * Math.sin(_r) - _offY;
		}
	}
}