package akdcl.skeleton{
	
	/**
	 * 事件帧数据
	 * @author Akdcl
	 */
	final public class EventFrame {
		/**
		 * 帧事件名
		 */
		public var name:String;
		
		/**
		 * 该事件发生在第几帧
		 */
		public var frame:uint;
		
		/**
		 * 构造函数
		 * @param _name 帧事件名
		 * @param _frame 该事件发生在第几帧
		 */
		public function EventFrame(_name:String, _frame:uint) {
			name = _name;
			frame = _frame;
		}
	}
}