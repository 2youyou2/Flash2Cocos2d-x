package akdcl.skeleton{
	
	/**
	 * 骨架动画数据
	 * @author Akdcl
	 */
	final public class ArmatureAniData {
		
		/**
		 * @private
		 */
		private var boneAniDataDic:Object;
		
		/**
		 * @private
		 */
		private var animationNames:Array;
		
		/**
		 * 构造函数
		 */
		public function ArmatureAniData() {
			boneAniDataDic = {};
			animationNames = [];
		}
		
		/**
		 * 删除，回收
		 */
		public function remove():void {
			/*for each(var _boneAniData:BoneAniData in boneAniDataDic) {
				_boneAniData.remove();
			}*/
			boneAniDataDic = null;
			animationNames = null;
		}
		
		/**
		 * 添加单个动作动画，比如 run，stand 等
		 * @param _boneAniData
		 * @param _animationName
		 */
		public function addAnimation(_boneAniData:BoneAniData, _animationName:String):void {
			boneAniDataDic[_animationName] = _boneAniData;
			animationNames.push(_animationName);
		}
		
		/**
		 * 获取单个动作动画，比如 run，stand 等
		 * @param _animationName
		 * @return 返回 BoneAniData 实例
		 */
		public function getAnimation(_animationName:String):BoneAniData {
			return boneAniDataDic[_animationName];
		}
		
		/**
		 * 获取拥有的动作动画列表
		 * @return 动画列表数组
		 */
		public function getAnimationNames():Array {
			return animationNames;
		}
	}
}