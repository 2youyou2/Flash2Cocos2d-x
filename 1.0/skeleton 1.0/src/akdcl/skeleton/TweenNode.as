package akdcl.skeleton
{
	/**
	 * 补间控制点
	 * @author Akdcl
	 */
	final public class TweenNode extends FrameNode {
		
		/**
		 * @private
		 */
		private var sR:Number;
		
		/**
		 * @private
		 */
		private var sX:Number;
		
		/**
		 * @private
		 */
		private var sY:Number;
		
		/**
		 * @private
		 */
		private var sSX:Number;
		
		/**
		 * @private
		 */
		private var sSY:Number;
		
		/**
		 * @private
		 */
		private var sA:Number;
		
		/**
		 * @private
		 */
		private var dR:Number;
		
		/**
		 * @private
		 */
		private var dX:Number;
		
		/**
		 * @private
		 */
		private var dY:Number;
		
		/**
		 * @private
		 */
		private var dSX:Number;
		
		/**
		 * @private
		 */
		private var dSY:Number;
		
		/**
		 * @private
		 */
		private var dA:Number;
		
		/**
		 * 构造函数
		 * @param _x
		 * @param _x
		 * @param _rotation
		 */
		public function TweenNode(_x:Number = 0, _y:Number = 0, _rotation:Number = 0) {
			super(_x, _y, _rotation);
			
		}
		
		/**
		 * 让 TweenNode 从两个关键点中取值
		 * @param _from
		 * @param _to
		 */
		public function betweenValue(_from:FrameNode, _to:FrameNode):void {
			sR = _from.rotation;
			sX = _from.x;
			sY = _from.y;
			sSX = _from.scaleX;
			sSY = _from.scaleY;
			sA = _from.alpha;
			
			if (_to.offR) {
				dR = _to.rotation + _to.offR * 360 - sR;
			}else {
				dR = _to.rotation - sR;
			}
			
			dX = _to.x - sX;
			dY = _to.y - sY;
			dSX = _to.scaleX - sSX;
			dSY = _to.scaleY - sSY;
			dA = _to.alpha - sA;
		}
		
		/**
		 * 让 TweenNode 从两个关键点中取值
		 * @param _k 0~1 的值，当此值为 0，TweenNode 取 from 点值，当此值为 1，TweenNode 取 to 点值
		 */
		public function tweenTo(_k:Number):void {
			rotation = sR + dR * _k;
			x = sX + dX * _k;
			y = sY + dY * _k;
			
			if (dSX) {
				scaleX = sSX + dSX * _k;
			}else {
				scaleX = NaN;
			}
			
			if (dSY) {
				scaleY = sSY + dSY * _k;
			}else {
				scaleY = NaN;
			}
			
			if (dA) {
				alpha = sA + dA * _k;
			}else {
				alpha = NaN;
			}
		}
	}
	
}