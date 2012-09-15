package akdcl.skeleton
{
	
	/**
	 * 关键帧数据
	 * @author Akdcl
	 */
	public class FrameNode extends Node {
		
		/**
		 * @private
		 */
		internal var scaleX:Number;
		
		/**
		 * @private
		 */
		internal var scaleY:Number;
		
		/**
		 * @private
		 */
		internal var alpha:Number;
		
		/**
		 * @private
		 */
		internal var offR:Number;
		
		/**
		 * @private
		 */
		internal var frame:uint;
		
		/**
		 * 构造函数
		 * @param _x
		 * @param _y
		 * @param _rotation
		 */
		public function FrameNode(_x:Number = 0, _y:Number = 0, _rotation:Number = 0) {
			super(_x, _y, _rotation);
			
			scaleX = 1;
			scaleY = 1;
			alpha = 1;
			frame = 1;
			
			offR = 0;
		}
		
		override public function copy(_fV:Node):void {
			super.copy(_fV);
			var _nV:FrameNode = _fV as FrameNode;
			if (_nV) {
				scaleX = _nV.scaleX;
				scaleY = _nV.scaleY;
				alpha = _nV.alpha;
				frame = _nV.frame;
				
				offR = _nV.offR;
			}
		}
	}
	
}