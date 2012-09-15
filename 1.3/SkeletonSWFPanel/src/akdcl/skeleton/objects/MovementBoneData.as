package akdcl.skeleton.objects {
	
	/**
	 * ...
	 * @author Akdcl
	 */
	final public class MovementBoneData extends BaseDicData {
		public var scale:Number;
		public var delay:Number;
		public var duration:int;
		public var length:int;
		
		public function MovementBoneData(_name:String = null, _scale:Number = 1, _delay:Number = 0) {
			super(_name);
			scale = _scale > 0?_scale:1;
			delay = (_delay || 0) % 1;
			if (delay > 0) {
				delay -= 1;
			}
			length = 0;
			duration = 0;
		}
		
		override public function addData(_data:Object, _id:String = null):void {
			var _frameData:FrameData = _data as FrameData;
			if (_frameData) {
				super.addData(_frameData, _id);
				length ++;
				duration += _frameData.duration;
			}
		}
		
		public function getData(_index:int):FrameData {
			return datas[_index];
		}
	}
	
}