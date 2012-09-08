package akdcl.skeleton
{
	import flash.geom.Point;
	
	/**
	 * 格式化、管理骨骼配置与骨骼动画
	 * @author Akdcl
	 */
	final public class ConnectionData {
		
		/**
		 * @private
		 */
		private static const BONE:String = "bone";
		
		/**
		 * @private
		 */
		private static const ANIMATION:String = "animation";
		
		/**
		 * @private
		 */
		private static const NAME:String = "name";
		
		/**
		 * @private
		 */
		private static const PARENT:String = "parent";
		
		/**
		 * @private
		 */
		private static const ROTATION:String = "rotation";
		
		/**
		 * @private
		 */
		private static const OFF_R:String = "offR";
		
		/**
		 * @private
		 */
		private static const X:String = "x";
		
		/**
		 * @private
		 */
		private static const Y:String = "y";
		
		/**
		 * @private
		 */
		private static const Z:String = "z";
		
		/**
		 * @private
		 */
		private static const SCALE_X:String = "scaleX";
		
		/**
		 * @private
		 */
		private static const SCALE_Y:String = "scaleY";
		
		/**
		 * @private
		 */
		private static const ALPHA:String = "alpha";
		
		/**
		 * @private
		 */
		private static const DELAY:String = "delay";
		
		/**
		 * @private
		 */
		private static const SCALE:String = "scale";
		
		/**
		 * @private
		 */
		private static const FRAME:String = "frame";
		
		/**
		 * @private
		 */
		private static const EVENT_FRAME:String = "eventFrame";
		
		/**
		 * 存储骨架数据 XMLList
		 * @private
		 */
		private static var armarureDatas:Object = { };
		
		/**
		 * 存储骨架动画数据 ArmatureAniData
		 * @private
		 */
		private static var animationDatas:Object = { };
		
		/**
		 * @param _id
		 * @return 返回骨架数据
		 */
		public static function getArmatureData(_id:String):XMLList {
			return armarureDatas[_id];
		}
		
		/**
		 * @param _id
		 * @return 返回骨架动画数据
		 */
		public static function getAnimationData(_id:String):ArmatureAniData {
			return animationDatas[_id];
		}
		
		/**
		 * 将XML数据转换成内置数据 ArmatureAniData 等
		 * @param _xml XML数据
		 */
		public static function addData(_xml:XML):void {
			var _name:String = _xml.attribute(NAME);
			//trace(_name);
			var _aniData:ArmatureAniData = armarureDatas[_name];
			if (_aniData) {
				return;
			}
			
			armarureDatas[_name] = _xml.elements(BONE);
			//trace(armarureDatas[_name]);
			animationDatas[_name] = _aniData = new ArmatureAniData();
			
			var _aniName:String;
			var _boneName:String;
			var _boneAniData:BoneAniData;
			var _frameXMLList:XMLList;
			var _animationList:XMLList = _xml.elements(ANIMATION);
			for each(var _eachAni:XML in _animationList) {
				_aniName = String(_eachAni.attribute(NAME));
				_boneAniData = new BoneAniData();
				_aniData.addAnimation(_boneAniData, _aniName);
				_boneAniData.frame = int(_eachAni.attribute(FRAME));
				_frameXMLList = _eachAni.elements(EVENT_FRAME);
				
				if (_frameXMLList.length() > 0) {
					_boneAniData.eventList = new Vector.<EventFrame>;
					var _eventFrame:EventFrame;
					var _frame:uint = 0;
					for each(_eachBoneAni in _frameXMLList){
						
						_eventFrame = new EventFrame(String(_eachBoneAni.attribute(NAME)), int(_eachBoneAni.attribute(FRAME)));
						_boneAniData.eventList.push(_eventFrame);
						_frame += _eventFrame.frame;
					}
					_boneAniData.eventList.unshift(new EventFrame("init", _boneAniData.frame - _frame));
				}
				
				for each(var _eachBoneAni:XML in _eachAni.children()) {
					_boneName = String(_eachBoneAni.name());
					
					if (_boneName != EVENT_FRAME) {
						if (_boneAniData.getAnimation(_boneName)) {
							continue;
						}
						_boneAniData.addAnimation(getFrameNodeList(_eachAni.elements(_boneName)), _boneName);
						//trace(_eachAni.elements(_boneName));
					}
				}
			}
			delete _xml[ANIMATION];
			delete _xml[BONE];
		}
		
		/**
		 * @private
		 */
		private static function getFrameNodeList(_frameXMLList:XMLList):FrameNodeList {
			var _nodeList:FrameNodeList = new FrameNodeList();
			_nodeList.scale = Number(_frameXMLList[0].attribute(SCALE)) || _nodeList.scale;
			_nodeList.delay = Number(_frameXMLList[0].attribute(DELAY)) || _nodeList.delay;
			
			
			for each(var _nodeXML:XML in _frameXMLList) {
				_nodeList.addFrame(getFrameNode(_nodeXML));
			}
			return _nodeList;
		}
		
		/**
		 * @private
		 */
		private static function getFrameNode(_nodeXML:XML):FrameNode {
			var _rotation:Number = Number(_nodeXML.attribute(ROTATION));
			//_rotation = _rotation * Math.PI / 180;
			var _node:FrameNode = new FrameNode(Number(_nodeXML.attribute(X)), Number(_nodeXML.attribute(Y)), _rotation);
			_node.scaleX = Number(_nodeXML.attribute(SCALE_X)) || _node.scaleX;
			_node.scaleY = Number(_nodeXML.attribute(SCALE_Y)) || _node.scaleY;
			_node.alpha = Number(_nodeXML.attribute(ALPHA)) || _node.alpha;
			_node.offR = Number(_nodeXML.attribute(OFF_R)) || _node.offR;
			//_node.offR = _node.offR * Math.PI / 180;
			_node.frame = int(_nodeXML.attribute(FRAME)) || _node.frame;
			return _node;
		}
	}
}