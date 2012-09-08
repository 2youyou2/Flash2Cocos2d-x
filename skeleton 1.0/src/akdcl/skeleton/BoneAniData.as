package akdcl.skeleton{
	
	/**
	 * 单个动作动画数据
	 * @author Akdcl
	 */
	final public class BoneAniData {
		
		/**
		 * 动画数据实际持续的帧数
		 */
		public var frame:uint;
		
		/**
		 * 事件帧列表
		 */
		public var eventList:Vector.<EventFrame>;
		
		/**
		 * @private
		 */
		private var frameNodeListDic:Object;
		
		/**
		 * 构造函数
		 */
		public function BoneAniData() {
			frameNodeListDic = { };
		}
		
		/**
		 * 删除，回收
		 */
		public function remove():void {
			frameNodeListDic = null;
			eventList = null;
		}
		
		/**
		 * 添加骨骼的动画数据
		 * @param _frameNodeList
		 * @param _boneName
		 */
		public function addAnimation(_frameNodeList:FrameNodeList, _boneName:String):void {
			frameNodeListDic[_boneName] = _frameNodeList;
		}
		
		/**
		 * 获取骨骼动画数据
		 * @param _boneName
		 * @return 返回骨骼动画数据
		 */
		public function getAnimation(_boneName:String):FrameNodeList {
			return frameNodeListDic[_boneName];
		}
	}
}