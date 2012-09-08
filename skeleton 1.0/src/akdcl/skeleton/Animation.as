package akdcl.skeleton{
	
	/**
	 * 为骨架管理Tween
	 * @author Akdcl
	 */
	final public class Animation extends ProcessBase {
		
		/**
		 * 播放开始
		 */
		public static const START:String = "start";
		
		/**
		 * 播放结束
		 */
		public static const COMPLETE:String = "complete";
		
		/**
		 * 循环结束
		 */
		public static const LOOP_COMPLETE:String = "loopComplete";
		
		/**
		 * 进入事件帧
		 */
		public static const IN_FRAME:String = "inFrame";
		
		/**
		 * 事件回调
		 */
		public var onAnimation:Function;
		
		/**
		 * @private
		 */
		private var armatureAniData:ArmatureAniData;
		
		/**
		 * @private
		 */
		private var boneAniData:BoneAniData;
		
		/**
		 * @private
		 */
		private var tweens:Object;
		
		/**
		 * @private
		 */
		private var aniIDNow:String;
		
		/**
		 * 构造函数
		 */
		public function Animation() {
		}
		
		/**
		 * 删除，回收
		 */
		override public function remove():void {
			stop();
			super.remove();
			for each(var _tween:Tween in tweens) {
				_tween.remove();
			}
			
			armatureAniData = null;
			boneAniData = null;
			aniIDNow = null;
			tweens = null;
		}
		
		override public function pause():void {
			super.pause();
			for each(var _tween:Tween in tweens) {
				_tween.pause();
			}
		}
		
		override public function resume():void 	{
			super.resume();
			for each(var _tween:Tween in tweens) {
				_tween.resume();
			}
		}
		
		override public function stop():void {
			super.stop();
			for each(var _tween:Tween in tweens) {
				_tween.stop();
			}
		}
		
		/**
		 * 获取当前 ArmatureAniData 
		 */
		public function getData():ArmatureAniData {
			return armatureAniData;
		}
		
		/**
		 * 设置 ArmatureAniData
		 */
		public function setData(_aniData:ArmatureAniData):void {
			remove();
			tweens = { };
			armatureAniData = _aniData;
		}
		
		/**
		 * 对骨骼动画的速度进行缩放
		 * @param _scale 缩放值做为系数乘到原来的动画帧值
		 * @param _boneName 指定某个缩放的骨骼ID，不指定则默认为所有骨骼
		 */
		public function setAnimationScale(_scale:Number, _boneName:String = null):void {
			var _tween:Tween;
			if (_boneName) {
				_tween = tweens[_boneName];
				if (_tween) {
					_tween.scale = _scale;
				}
			}else {
				scale = _scale;
				for each(_tween in tweens) {
					_tween.scale = _scale;
				}
			}
		}
		
		/**
		 * 为骨骼添加 Tween 实例
		 */
		public function addTween(_bone:Bone):void {
			if (!tweens) {
				tweens = { };
			}
			var _boneID:String = _bone.name;
			var _tween:Tween = tweens[_boneID];
			if (!_tween) {
				tweens[_boneID] = _tween = Tween.create();
			}
			_tween.setNode(_bone.tweenNode);
		}
		
		/**
		 * 删除骨骼的 Tween 实例
		 */
		public function removeTween(_bone:Bone):void {
			var _boneID:String = _bone.name;
			var _tween:Tween = tweens[_boneID];
			if (_tween) {
				delete tweens[_boneID];
				_tween.remove();
			}
		}
		
		/**
		 * 获取骨骼的 Tween 实例
		 */
		public function getTween(_boneID:String):Tween {
			return tweens[_boneID];
		}
		
		/**
		 * 更新指定的 Tween 实例
		 */
		public function updateTween(_boneID:String):void {
			var _tween:Tween = tweens[_boneID];
			if (_tween) {
				_tween.update();
			}
		}
		
		/**
		 * 播放动画
		 * @param _to ArmatureAniData 在 ConnectionData 中的字符串索引
		 * @param _toFrames 过渡到 _to 动画需要花费的帧
		 * @param _listFrames _to 动画播放持续的帧
		 * @param _loop 是否循环
		 * @param _ease FrameNodeList 各个关键点的缓动方式，-1:SineOut，0:Line，1:SineIn，2:SineInOut
		 */
		override public function playTo(_to:Object, _toFrames:uint, _listFrames:uint = 0, _loop:Boolean = false, _ease:int = 0):void {
			if (!armatureAniData) {
				return;
			}
			boneAniData = armatureAniData.getAnimation(_to as String);
			if (!boneAniData) {
				return;
			}
			super.playTo(_to, _toFrames, _listFrames, _loop, _ease);
			aniIDNow = _to as String;
			var _eachA:FrameNodeList;
			var _tween:Tween;
			for (var _boneName:String in tweens) {
				_tween = tweens[_boneName];
				_eachA = boneAniData.getAnimation(_boneName);
				if (_eachA) {
					_tween.playTo(_eachA, _toFrames, _listFrames, _loop, _ease);
				}
			}
			noScaleListFrames = boneAniData.frame;
			
			if (noScaleListFrames == 1) {
				//普通过渡
				loop = -4;
			}else {
				if (_loop) {
					//循环过渡
					loop = -2;
				}else {
					//列表过渡
					loop = -3;
					noScaleListFrames --;
				}
				listFrames = _listFrames;
			}
		}
		
		/**
		 * @private
		 */
		override protected function updateHandler():void {
			if (currentPrecent >= 1) {
				switch(loop) {
					case -3:
						//列表过渡
						loop = -1;
						currentPrecent = (currentPrecent - 1) * totalFrames / listFrames;
						if (currentPrecent >= 1) {
							//
						}else {
							currentPrecent %= 1;
							totalFrames = listFrames;
							listEndFrame = 0;
							if (onAnimation!=null) {
								onAnimation(START, aniIDNow);
							}
							break;
						}
					case -1:
					case -4:
						currentPrecent = 1;
						isComplete = true;
						if (onAnimation!=null) {
							onAnimation(COMPLETE, aniIDNow);
						}
						break;
					case -2:
						//循环开始
						loop = 0;
						currentPrecent %= 1;
						totalFrames = listFrames;
						listEndFrame = 0;
						if (onAnimation!=null) {
							onAnimation(START, aniIDNow);
						}
						break;
					default:
						//继续循环
						loop += int(currentPrecent);
						currentPrecent %= 1;
						if (onAnimation != null) {
							onAnimation(LOOP_COMPLETE, aniIDNow);
						}
						break;
				}
			}
			
			if (loop >= -1 && boneAniData.eventList) {
				//多关键帧动画过程
				updateCurrentPrecent();
			}
		}
		
		/**
		 * @private
		 */
		private function updateCurrentPrecent():void {
			var _playedFrames:Number = noScaleListFrames * currentPrecent;
			if (_playedFrames <= listEndFrame-betweenFrame || _playedFrames > listEndFrame) {
				toFrameID = 0;
				listEndFrame = 0;
				
				var _prevFrameID:int;
				do {
					betweenFrame = boneAniData.eventList[toFrameID].frame;
					listEndFrame += betweenFrame;
					_prevFrameID = toFrameID;
					if (++toFrameID>=boneAniData.eventList.length) {
						toFrameID = 0;
					}
				}while (_playedFrames >= listEndFrame);
				
				if (onAnimation != null) {
					onAnimation(IN_FRAME, aniIDNow, boneAniData.eventList[_prevFrameID].name);
				}
			}
			
			currentPrecent = 1 - (listEndFrame - _playedFrames) / betweenFrame;
		}
	}
	
}