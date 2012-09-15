package akdcl.skeleton.objects {
	
	/**
	 * ...
	 * @author Akdcl
	 */
	final public class FrameData extends Node {
		public var duration:int;
		
		public var tweenEasing:Number;
		
		public var hide:Boolean;
		public var image:String;
		public var isArmature:Boolean;
		public var movement:String;
		
		public var event:String;
		
		public var sound:String;
		public var soundEffect:String;
		
		public function FrameData(
			_x:Number = 0, 
			_y:Number = 0, 
			_skewX:Number = 0, 
			_skewY:Number = 0, 
			_scaleX:Number = 1, 
			_scaleY:Number = 1, 
			_duration:int = 1,
			_tweenEasing:Number = 0
		) {
			super(_x, _y, _skewX, _skewY, _scaleX, _scaleY);
			
			duration = _duration > 0?_duration:1;
			
			//为NaN则不会补间，-1~0~1~2淡出、线性、淡入、淡入淡出
			tweenEasing = _tweenEasing;
		}
		
		public function dispose():void {
			
		}
	}
	
}