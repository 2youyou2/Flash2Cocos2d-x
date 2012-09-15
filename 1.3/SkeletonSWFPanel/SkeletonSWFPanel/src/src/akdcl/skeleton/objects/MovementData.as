package akdcl.skeleton.objects {
	
	/**
	 * ...
	 * @author Akdcl
	 */
	final public class MovementData extends BaseDicData {
		public var duration:int;
		public var durationTo:int;
		public var durationTween:int;
		public var loop:Boolean;
		public var tweenEasing:Number;
		
		public function MovementData(_name:String = null, _duration:int = 1, _durationTo:int = 0, _durationTween:int = 0, _loop:Boolean = false, _tweenEasing:Number = NaN) {
			super(_name);
			duration = _duration > 0?_duration:1;
			durationTo = _durationTo >= 0?_durationTo:0;
			durationTween = _durationTween >= 0?_durationTween:0;
			loop = _loop;
			//为NaN则不启用
			tweenEasing = _tweenEasing;
		}
		
		public function getData(_name:String):MovementBoneData {
			return datas[_name];
		}
	}
	
}