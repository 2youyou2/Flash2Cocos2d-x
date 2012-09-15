package akdcl.skeleton.objects
{
	
	/**
	 * ...
	 * @author Akdcl
	 */
	public class Node {
		private static const DOUBLE_PI:Number = Math.PI * 2;
		public var x:Number;
		public var y:Number;
		public var scaleX:Number;
		public var scaleY:Number;
		public var skewX:Number;
		public var skewY:Number;
		public var z:Number;
		
		public var tweenRotate:int;
		
		public function get rotation():Number {
			return skewY;
		}
		public function set rotation(_rotation:Number):void {
			skewX = skewY = _rotation;
		}
		
		public function Node(_x:Number = 0, _y:Number = 0, _skewX:Number = 0, _skewY:Number = 0, _scaleX:Number = 1, _scaleY:Number = 1) {
			x = _x || 0;
			y = _y || 0;
			skewX = _skewX || 0;
			skewY = _skewY || 0;
			scaleX = _scaleX || 1;
			scaleY = _scaleY || 1;
			
			tweenRotate = 0;
			
			z = 0;
		}
		
		final public function copy(_node:Node):void {
			x = _node.x;
			y = _node.y;
			scaleX = _node.scaleX;
			scaleY = _node.scaleY;
			skewX = _node.skewX;
			skewY = _node.skewY;
			
			tweenRotate = _node.tweenRotate;
			
			z = _node.z;
		}
		
		final public function subtract(_from:Node, _to:Node):void {
			x = _to.x - _from.x;
			y = _to.y - _from.y;
			scaleX = _to.scaleX - _from.scaleX;
			scaleY = _to.scaleY - _from.scaleY;
			skewX = _to.skewX - _from.skewX;
			skewY = _to.skewY - _from.skewY;
			if (_to.tweenRotate) {
				skewX += _to.tweenRotate * DOUBLE_PI;
			}else {
				skewX %= DOUBLE_PI;
				if (skewX > Math.PI) {
					skewX -= DOUBLE_PI;
				}
				if (skewX < -Math.PI) {
					skewX += DOUBLE_PI;
				}
			}
			if (_to.tweenRotate) {
				skewY += _to.tweenRotate * DOUBLE_PI;
			}else {
				skewY %= DOUBLE_PI;
				if (skewY > Math.PI) {
					skewY -= DOUBLE_PI;
				}
				if (skewY < -Math.PI) {
					skewY += DOUBLE_PI;
				}
			}
		}
		
		public function toString():String {
			var _str:String = "";
			_str += "x:" + x + " y:" + y + " rotation:" + rotation;
			return _str;
		}
		
	}
	
}