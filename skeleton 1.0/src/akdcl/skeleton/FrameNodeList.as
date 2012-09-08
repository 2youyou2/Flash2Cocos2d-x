package akdcl.skeleton
{
	/**
	 * 关键帧数据列表
	 * @author akdcl
	 */
	final public class FrameNodeList {
		
		/**
		 * 播放延时百分比
		 */
		public var delay:Number;
		
		/**
		 * 周期缩放百分比
		 */
		public var scale:Number;
		
		/**
		 * 实际持续帧数
		 */
		public var frame:uint;
		
		/**
		 * 列表长度
		 */
		public var length:uint;
		
		/**
		 * @private
		 */
		private var list:Vector.<FrameNode>;
		
		/**
		 * 构造函数
		 * @param _delay 播放延时百分比
		 * @param _scale 周期缩放百分比
		 */
		public function FrameNodeList(_delay:Number = 0, _scale:Number = 1) {
			delay = _delay;
			scale = _scale;
			frame = 0;
			length = 0;
			
			list = new Vector.<FrameNode>;
		}
		
		/**
		 * 添加关键帧
		 * @param _value 关键帧数据
		 */
		public function addFrame(_value:FrameNode):void {
			list.push(_value);
			frame += _value.frame;
			length++;
		}
		
		/**
		 * 获取关键帧
		 * @param _id index
		 * @return 返回关键帧数据
		 */
		public function getFrame(_id:int):FrameNode {
			if (_id<0) {
				_id = length + _id;
			}
			return list[_id];
		}
	}
	
}