package akdcl.skeleton.objects{
	
	/**
	 * ...
	 * @author Akdcl
	 */
	final public class BoneData extends Node {
		public var name:String;
		public var parent:String;
		
		public var hide:Boolean;
		public var image:String;
		public var isArmature:Boolean;
		
		public function BoneData(_x:Number = 0, _y:Number = 0, _skewX:Number = 0, _skewY:Number = 0) {
			super(_x, _y, _skewX, _skewY);
		}
	}
	
}