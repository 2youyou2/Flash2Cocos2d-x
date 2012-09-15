package akdcl.skeleton {
	
	/**
	 * 动画基类
	 * @author Akdcl
	 */
	public class ProcessBase {
		/**
		 * 动画播放速度缩放值，默认为1
		 */
		public var scale:Number;
		
		/**
		 * @private
		 */
		protected var isPause:Boolean;
		
		/**
		 * @private
		 */
		protected var isComplete:Boolean;
		
		/**
		 * @private
		 */
		protected var currentFrame:Number;
		
		/**
		 * @private
		 */
		protected var totalFrames:uint;
		
		/**
		 * @private
		 */
		protected var listFrames:uint;
		
		/**
		 * @private
		 */
		protected var noScaleListFrames:uint;
		
		/**
		 * @private
		 */
		protected var currentPrecent:Number;
		
		/**
		 * @private
		 */
		protected var loop:int;
		
		/**
		 * @private
		 */
		protected var ease:int;
		
		/**
		 * @private
		 */
		protected var toFrameID:int;
		
		/**
		 * @private
		 */
		protected var betweenFrame:uint;
		
		/**
		 * @private
		 */
		protected var listEndFrame:uint;
		
		/**
		 * 构造函数
		 */
		public function ProcessBase() {
			scale = 1;
			isComplete = true;
			isPause = false;
			currentFrame = 0;
		}
		
		/**
		 * 删除，回收
		 */
		public function remove():void {
			scale = 1;
			isComplete = true;
			isPause = false;
			currentFrame = 0;
		}
		
		/**
		 * 暂停
		 */
		public function pause():void {
			isPause = true;
		}
		
		/**
		 * 继续
		 */
		public function resume():void {
			isPause = false;
		}
		
		/**
		 * 停止
		 */
		public function stop():void {
			isComplete = true;
			currentFrame = 0;
		}
		
		/**
		 * 播放到
		 */
		public function playTo(_to:Object, _toFrames:uint, _listFrames:uint = 0, _loop:Boolean = false, _ease:int = 0):void {
			isComplete = false;
			isPause = false;
			currentFrame = 0;
			totalFrames = _toFrames;
			ease = _ease;
		}
		
		/**
		 * 更新
		 */
		final public function update():void {
			if (isComplete || isPause) {
				return;
			}
			currentFrame += scale;
			currentPrecent = currentFrame / totalFrames;
			currentFrame %= totalFrames;
			updateHandler();
		}
		
		/**
		 * @private
		 */
		protected function updateHandler():void {
		}
	}
	
}