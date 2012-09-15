package akdcl.skeleton.animation {
	
	/**
	 * 
	 * @author Akdcl
	 */
	internal class ProcessBase {
		protected static const SINGLE:int = -4;
		protected static const LIST_START:int = -3;
		protected static const LIST_LOOP_START:int = -2;
		protected static const LIST:int = -1;
		
		protected var isPause:Boolean;
		protected var isComplete:Boolean;
		
		protected var currentFrame:Number;
		protected var totalFrames:int;
		protected var currentPrecent:Number;
		
		protected var durationTween:int;
		protected var duration:int;
		
		protected var loop:int;
		protected var tweenEasing:int;
		
		protected var toIndex:int;
		protected var betweenDuration:int;
		protected var totalDuration:int;
		
		protected var __scale:Number;
		public function get scale():Number {
			return __scale;
		}
		public function set scale(_scale:Number):void {
			__scale = _scale;
		}
		
		public function ProcessBase() {
			__scale = 1;
			isComplete = true;
			isPause = false;
			currentFrame = 0;
		}
		
		public function play(_animation:Object, _durationTo:int = 0, _durationTween:int = 0, _loop:* = false, _tweenEasing:Number = NaN):void {
			isComplete = false;
			isPause = false;
			currentFrame = 0;
			totalFrames = _durationTo;
			tweenEasing = _tweenEasing;
		}
		
		public function pause():void {
			isPause = true;
		}
		
		public function resume():void {
			isPause = false;
		}
		
		public function stop():void {
			isComplete = true;
			currentFrame = 0;
		}
		
		final public function update():void {
			if (isComplete || isPause) {
				return;
			}
			if (totalFrames <= 0) {
				currentFrame = totalFrames = 1;
			}
			currentFrame += __scale;
			currentPrecent = currentFrame / totalFrames;
			currentFrame %= totalFrames;
			updateHandler();
		}
		
		protected function updateHandler():void {
			
		}
	}
	
}