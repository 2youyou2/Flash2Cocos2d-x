package akdcl.skeleton.animation{
	import akdcl.skeleton.utils.skeletonNamespace;
	
	import akdcl.skeleton.Armature;
	import akdcl.skeleton.Bone;
	import akdcl.skeleton.objects.AnimationData;
	import akdcl.skeleton.objects.MovementData;
	import akdcl.skeleton.objects.MovementBoneData;
	
	/**
	 * 
	 * @author Akdcl
	 */
	final public class Animation extends ProcessBase {
		public var movementList:Array;
		public var movementID:String;
		
		protected var updateDisplayFun:Function;
		protected var containerFun:Function;
		
		private var animationData:AnimationData;
		private var movementData:MovementData;
		
		private var armature:Armature;
		private var bones:Object;
		
		override public function set scale(_scale:Number):void {
			super.scale = _scale;
			for each(var _bone:Bone in bones) {
				_bone.tween.scale = _scale;
			}
		}
		
		public function Animation(_armature:Armature) {
			armature = _armature;
			bones = armature.skeletonNamespace::bones;
		}
		
		public function setData(_animationData:AnimationData):void {
			if (!_animationData) {
				return;
			}
			stop();
			animationData = _animationData;
			movementList = animationData.getSearchList();
		}
		
		override public function play(_movementID:Object, _durationTo:int = -1, _durationTween:int = -1, _loop:* = null, _tweenEasing:Number = NaN):void {
			if (!animationData) {
				return;
			}
			var _movementData:MovementData = animationData.getData(_movementID as String);
			if (!_movementData) {
				return;
			}
			totalDuration = 0;
			toIndex = 0;
			movementID = _movementID as String;
			movementData = _movementData;
			_durationTo = _durationTo < 0?movementData.durationTo:_durationTo;
			_durationTween = _durationTween < 0?movementData.durationTween:_durationTween;
			_loop = _loop === null?movementData.loop:_loop;
			_tweenEasing = isNaN(_tweenEasing)?movementData.tweenEasing:_tweenEasing;
			
			super.play(null, _durationTo, _durationTween);
			duration = movementData.duration;
			if (duration == 1) {
				loop = SINGLE;
			}else {
				if (_loop) {
					loop = LIST_LOOP_START
				}else {
					loop = LIST_START
					duration --;
				}
				durationTween = _durationTween;
			}
			
			var _movementBoneData:MovementBoneData;
			for each(var _bone:Bone in bones) {
				_movementBoneData = movementData.getData(_bone.info.name);
				if (_movementBoneData) {
					_bone.tween.play(_movementBoneData, _durationTo, _durationTween, _loop, _tweenEasing);
				}else {
					_bone.recycleDisplay();
					_bone.tween.stop();
				}
			}
		}
		
		override public function pause():void {
			super.pause();
			for each(var _bone:Bone in bones) {
				_bone.tween.pause();
			}
		}
		
		override public function resume():void {
			super.resume();
			for each(var _bone:Bone in bones) {
				_bone.tween.resume();
			}
		}
		
		override public function stop():void {
			super.stop();
			for each(var _bone:Bone in bones) {
				_bone.tween.stop();
			}
		}
		
		override protected function updateHandler():void {
			if (currentPrecent >= 1) {
				switch(loop) {
					case LIST_START:
						loop = LIST;
						currentPrecent = (currentPrecent - 1) * totalFrames / durationTween;
						if (currentPrecent >= 1) {
							//播放速度太快或durationTween时间太短，进入下面的case
						}else {
							totalFrames = durationTween;
							totalDuration = 0;
							armature.factory.animationEventHandler(armature, "start", movementID);
							break;
						}
					case LIST:
					case SINGLE:
						currentPrecent = 1;
						isComplete = true;
						armature.factory.animationEventHandler(armature, "complete", movementID);
						break;
					case LIST_LOOP_START:
						loop = 0;
						currentPrecent %= 1;
						totalFrames = durationTween;
						totalDuration = 0;
						armature.factory.animationEventHandler(armature, "start", movementID);
						break;
					default:
						//继续循环
						loop += int(currentPrecent);
						currentPrecent %= 1;
						totalDuration = 0;
						armature.factory.animationEventHandler(armature, "loopComplete", movementID);
						break;
				}
			}
			if (loop >= LIST) {
				//多关键帧动画过程
				//updateFrameData(currentPrecent);
			}
		}
		
		private function updateFrameData(_currentPrecent:Number):void {
			/*var _length:int = movementData.length;
			var _played:Number = duration * _currentPrecent;
			var _fromIndex:int;
			var _from:FrameData;
			var _to:FrameData;
			//播放头到达当前帧的前面或后面则重新寻找当前帧
			if (_played <= totalDuration - betweenDuration || _played > totalDuration) {
				do {
					betweenDuration = movementData.getFrame(toIndex).duration;
					totalDuration += betweenDuration;
					_fromIndex = toIndex;
					if (++toIndex >= _length) {
						toIndex = 0;
					}
				}while (_played >= totalDuration);
				_fromIndex, toIndex
				if (onAnimation != null) {
					onAnimation(IN_FRAME, aniIDNow, movementData.getFrame[_fromIndex]);
				}
			}*/
		}
	}
	
}