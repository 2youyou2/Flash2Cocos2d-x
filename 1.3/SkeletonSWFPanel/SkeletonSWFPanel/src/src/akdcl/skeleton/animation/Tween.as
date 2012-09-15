package akdcl.skeleton.animation {
	import akdcl.skeleton.Bone;
	import akdcl.skeleton.objects.FrameData;
	import akdcl.skeleton.objects.MovementBoneData;
	import akdcl.skeleton.objects.Node;
	
	/**
	 * 
	 * @author Akdcl
	 */
	final public class Tween extends ProcessBase {
		private static const HALF_PI:Number = Math.PI * 0.5;
		
		private var bone:Bone;
		
		public var node:Node;
		private var from:Node;
		private var between:Node;
		
		private var movementBoneData:MovementBoneData;
		private var frameTweenEasing:Number;
		
		private var currentKeyFrame:FrameData;
		private var nextKeyFrame:FrameData;
		private var isTweenKeyFrame:Boolean;
		
		public function Tween(_bone:Bone) {
			super();
			bone = _bone;
			node = new Node();
			from = new Node();
			between = new Node();
		}
		
		override public function play(_movementBoneData:Object, _durationTo:int = 0, _durationTween:int = 0, _loop:* = false, _tweenEasing:Number = NaN):void {
			currentKeyFrame = null;
			nextKeyFrame = null;
			isTweenKeyFrame = false;
			super.play(null, _durationTo, _durationTween, _loop, _tweenEasing);
			movementBoneData = _movementBoneData as MovementBoneData;
			//
			totalDuration = 0;
			betweenDuration = 0;
			toIndex = 0;
			node.skewY %= 360;
			var _frameData:FrameData;
			if (movementBoneData.length == 1) {
				loop = SINGLE;
				nextKeyFrame = movementBoneData.getData(0);
				setBetween(node, nextKeyFrame);
				isTweenKeyFrame = true;
				frameTweenEasing = 1;
			}else if (movementBoneData.length > 1) {
				if (_loop) {
					loop = LIST_LOOP_START;
					duration = movementBoneData.duration;
				}else {
					loop = LIST_START;
					duration = movementBoneData.duration - 1;
				}
				durationTween = _durationTween * movementBoneData.scale;
				if (_loop && movementBoneData.delay != 0) {
					setBetween(node, tweenNodeTo(updateFrameData(1 -movementBoneData.delay), between));
				}else {
					nextKeyFrame = movementBoneData.getData(0);
					setBetween(node, nextKeyFrame);
					isTweenKeyFrame = true;
				}
			}
		}
		
		override protected function updateHandler():void {
			if (currentPrecent >= 1) {
				switch(loop) {
					case SINGLE:
						currentKeyFrame = nextKeyFrame;
						currentPrecent = 1;
						isComplete = true;
						break;
					case LIST_START:
						loop = LIST;
						if (durationTween <= 0) {
							currentPrecent = 1;
						}else {
							currentPrecent = (currentPrecent - 1) * totalFrames / durationTween;
						}
						if (currentPrecent >= 1) {
							//播放速度太快或durationTween时间太短
							currentPrecent = 1;
							isComplete = true;
							break;
						}else {
							totalFrames = durationTween;
							totalDuration = 0;
							break;
						}
					case LIST:
						currentPrecent = 1;
						isComplete = true;
						break;
					case LIST_LOOP_START:
						loop = 0;
						totalFrames = durationTween > 0?durationTween:1;
						if (movementBoneData.delay != 0) {
							//
							currentFrame = (1 - movementBoneData.delay) * totalFrames;
							currentPrecent += currentFrame / totalFrames;
						}
						currentPrecent %= 1;
						//totalDuration = 0;
						//betweenDuration = 0;
						//toIndex = 0;
						break;
					default:
						//循环
						loop += int(currentPrecent);
						currentPrecent %= 1;
						
						totalDuration = 0;
						betweenDuration = 0;
						toIndex = 0;
						
						break;
				}
			}else if (loop < -1) {
				currentPrecent = Math.sin(currentPrecent * HALF_PI);
			}
			if (loop >= LIST) {
				//多关键帧动画过程
				currentPrecent = updateFrameData(currentPrecent, true);
			}
			if (!isNaN(frameTweenEasing)) {
				tweenNodeTo(currentPrecent);
			}else if(currentKeyFrame) {
				tweenNodeTo(0);
			}
			if(currentKeyFrame){
				//arrived
				bone.factory.boneKeyFrameRender(bone, currentKeyFrame);
				currentKeyFrame = null;
			}
			if(isTweenKeyFrame){
				//to
				if(nextKeyFrame.hide){
					bone.recycleDisplay();
				}
				isTweenKeyFrame = false;
			}
		}
		
		private function setBetween(_from:Node, _to:Node):void {
			from.copy(_from);
			between.subtract(_from, _to);
		}
		
		private function tweenNodeTo(_value:Number, _node:Node = null):Node {
			_node = _node || node;
			_node.x = from.x + _value * between.x;
			_node.y = from.y + _value * between.y;
			_node.scaleX = from.scaleX + _value * between.scaleX;
			_node.scaleY = from.scaleY + _value * between.scaleY;
			_node.skewX = from.skewX + _value * between.skewX;
			_node.skewY = from.skewY + _value * between.skewY;
			return _node;
		}
		
		private function updateFrameData(_currentPrecent:Number, _activeFrame:Boolean = false):Number {
			var _played:Number = duration * _currentPrecent;
			var _length:int = movementBoneData.length;
			var _fromIndex:int;
			var _from:FrameData;
			var _to:FrameData;
			var _isListEnd:Boolean;
			//播放头到达当前帧的前面或后面则重新寻找当前帧
			if (_played <= totalDuration - betweenDuration || _played > totalDuration) {
				do {
					betweenDuration = movementBoneData.getData(toIndex).duration;
					totalDuration += betweenDuration;
					_fromIndex = toIndex;
					if (++toIndex >= _length) {
						toIndex = 0;
					}
				}while (_played >= totalDuration);
				_isListEnd = loop == LIST && toIndex == 0;
				if(_isListEnd){
					_to = _from = movementBoneData.getData(_fromIndex);
				}else{
					_from = movementBoneData.getData(_fromIndex);
					_to = movementBoneData.getData(toIndex);
				}
				frameTweenEasing = _from.tweenEasing;
				if (_activeFrame) {
					currentKeyFrame = nextKeyFrame;
					if(!_isListEnd){
						nextKeyFrame = _to;
						isTweenKeyFrame = true;
					}
				}
				setBetween(_from, _to);
			}
			_currentPrecent = 1 - (totalDuration - _played) / betweenDuration;
			
			//frameTweenEasing为NaN则不会补间，-1~0~1~2、淡出、线性、淡入、淡入淡出
			var _tweenEasing:Number;
			if (!isNaN(frameTweenEasing)) {
				_tweenEasing = isNaN(tweenEasing)?frameTweenEasing:tweenEasing;
				if (_tweenEasing) {
					_currentPrecent = getEaseValue(_currentPrecent, _tweenEasing);
				}
			}
			return _currentPrecent;
		}
		
		private static function getEaseValue(_value:Number, _easing:Number):Number {
			if (_easing > 1) {
				_value = 0.5 * (1 - Math.cos(_value * Math.PI ));
				_easing -= 1;
			}else if (_easing > 0) {
				_value = Math.sin(_value * HALF_PI);
			}else {
				_value = 1 - Math.cos(_value * HALF_PI);
				_easing = -_easing;
			}
			return _value * _easing + (1 - _easing);
		}
	}
}