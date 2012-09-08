package akdcl.skeleton{
	import flash.geom.Point;
	
	/**
	 * 骨骼
	 * @author akdcl
	 */
	final public class Bone {
		
		/**
		 * @private
		 */
		private static const ANGLE_TO_RADIAN:Number = Math.PI / 180;
		
		/**
		 * @private
		 */
		private static var prepared:Vector.<Bone> = new Vector.<Bone>;
		
		/**
		 * 创建Bone实例，Bone 做为常用的实例，用对象池管理创建回收
		 * @param _isRadian 骨骼旋转角度是否采用弧度制，比如 Starling 使用的是弧度制
		 * @return 返回 Bone 实例
		 */
		public static function create(_isRadian:Boolean = false):Bone {
			if (prepared.length > 0) {
				return prepared.pop();
			}
			return new Bone(_isRadian);
		}
		
		/**
		 * @private
		 */
		private static function recycle(_bone:Bone):void {
			if (Bone.prepared.indexOf(_bone) < 0) {
				return;
			}
			Bone.prepared.push(_bone);
		}
		
		/**
		 * 骨骼名称 Armature 通过骨骼名索引 Bone。Animation 通过骨骼名建立 Bone 与 Tween 的关联
		 */
		public var name:String;
		
		/**
		 * 骨骼绑定的显示对象，并不是必须的，可以不绑定显示对象
		 */
		public var display:Object;
		
		/**
		 * 骨骼受控制的关键点
		 */
		public var node:Node;
		
		/**
		 * 受 Tween 控制的关键点，骨骼由 Tween 和自身 node 的合值控制
		 * @private
		 */
		internal var tweenNode:TweenNode;
		
		/**
		 * @private
		 */
		private var isRadian:Boolean;
		
		/**
		 * @private
		 */
		private var parent:Bone;
		
		/**
		 * @private
		 */
		private var transformX:Number;
		
		/**
		 * @private
		 */
		private var transformY:Number;
		
		/**
		 * @private
		 */
		private var lockX:Number;
		
		/**
		 * @private
		 */
		private var lockY:Number;
		
		/**
		 * @private
		 */
		private var lockR:Number;
		
		/**
		 * @private
		 */
		private var parentX:Number;
		
		/**
		 * @private
		 */
		private var parentY:Number;
		
		/**
		 * @private
		 */
		private var parentR:Number;
		
		/**
		 * 构造函数
		 * @param _isRadian 骨骼旋转角度是否采用弧度制，比如 Starling 使用的是弧度制
		 */
		public function Bone(_isRadian:Boolean = false) {
			isRadian = _isRadian;
			transformX = 0;
			transformY = 0;
			parentX = 0;
			parentY = 0;
			parentR = 0;
			lockX = 0;
			lockY = 0;
			lockR = 0;
			
			node = new Node();
			tweenNode = new TweenNode();
		}
		
		/**
		 * 删除，回收
		 */
		public function remove():void {
			display = null;
			name = null;
			parent = null;
			transformX = 0;
			transformY = 0;
			parentX = 0;
			parentY = 0;
			parentR = 0;
			lockX = 0;
			lockY = 0;
			lockR = 0;
			Bone.recycle(this);
		}
		
		/**
		 * 添加子骨骼
		 */
		public function addChild(_child:Bone):void{
			_child.parent = this;
		}
		
		/**
		 * 调整在父骨骼中的绑定位置，仅在拥有 parent 时起作用
		 */
		public function setLockPosition(_x:Number, _y:Number, _r:Number = 0):void {
			lockX = _x;
			lockY = _y;
			lockR = _r;
		}
		
		/**
		 * 更新位置
		 */
		public function update():void {
			if (parent) {
				parentX = parent.getGlobalX();
				parentY = parent.getGlobalY();
				parentR = parent.getGlobalR();
				
				var _dX:Number = lockX + node.x + tweenNode.x;
				var _dY:Number = lockY + node.y + tweenNode.y;
				var _r:Number = Math.atan2(_dY, _dX) + parentR * ANGLE_TO_RADIAN;
				//if( name=="weapon" || name=="body" || name=="armR")
				//trace(  name +": "+ _dY  +" "+ _dX  +" "+ parentR  +" "+ _r );
				var _len:Number = Math.sqrt(_dX * _dX + _dY * _dY);
				transformX = _len * Math.cos(_r);
				transformY = _len * Math.sin(_r);
			}else {
				transformX = node.x + tweenNode.x;
				transformY = node.y + tweenNode.y;
				//trace( name + "  " +  transformX + " "+ transformY + "  "+(node.rotation + tweenNode.rotation + parentR + lockR));  
			}
			
			updateDisplay();
		}
		
		/**
		 * 更新 display
		 * @private
		 */
		private function updateDisplay():void {
			if (display) {
				display.x = transformX + parentX;
				display.y = transformY + parentY;
				if (isRadian) {
					display.rotation = (node.rotation + tweenNode.rotation + parentR + lockR) * ANGLE_TO_RADIAN;
				}else {
					display.rotation = node.rotation + tweenNode.rotation + parentR + lockR;
				}
				
				if (isNaN(tweenNode.scaleX)) {
				}else {
					display.scaleX = tweenNode.scaleX;
				}
				if (isNaN(tweenNode.scaleY)) {
				}else {
					display.scaleY = tweenNode.scaleY;
				}
				if (isNaN(tweenNode.alpha)) {
				}else {
					if (tweenNode.alpha) {
						display.visible = true;
						display.alpha = tweenNode.alpha;
					}else {
						display.visible = false;
					}
				}
			}
		}
		
		/**
		 * 获取在 Armatur.display 中的全局 x 坐标
		 * @private
		 */
		internal function getGlobalX():Number {
			return transformX + parentX;
		}
		
		/**
		 * 获取在 Armatur.display 中的全局 y 坐标
		 * @private
		 */
		internal function getGlobalY():Number {
			return transformY + parentY;
		}
		
		/**
		 * 获取在 Armatur.display 中的全局 rotation
		 * @private
		 */
		internal function getGlobalR():Number {
			return node.rotation + tweenNode.rotation + parentR + lockR;
		}
	}
	
}