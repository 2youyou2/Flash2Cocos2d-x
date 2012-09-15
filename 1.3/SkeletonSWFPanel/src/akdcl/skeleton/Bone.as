package akdcl.skeleton {
	import akdcl.skeleton.animation.Tween;
	import akdcl.skeleton.factorys.BaseFactory;
	import akdcl.skeleton.objects.BoneData;
	import akdcl.skeleton.objects.Node;
	
	/**
	 * 
	 * @author akdcl
	 */
	public class Bone {
		public var userData:Object;
		public var factory:BaseFactory;
		
		public var info:BoneData;
		public var node:Node;
		public var tween:Tween;
		
		public var armature:Armature;
		public var parent:Bone;
		
		protected var children:Vector.<Bone>;
		
		private var tweenNode:Node;
		
		private var originX:Number;
		private var originY:Number;
		private var originSkewX:Number;
		private var originSkewY:Number;
		
		private var displays:Object;
		
		protected var __display:Object;
		public function get display():Object {
			return __display;
		}
		public function set display(_display:Object):void {
			if(__display && __display == _display) {
				return;
			}
			if (armature) {
				armature.factory.displayContainer(__display);
				armature.factory.displayContainer(_display, armature.display, info.z);
			}
			__display = _display;
		}
		
		public function Bone(_factory:BaseFactory) {
			factory = _factory;
			
			originX = 0;
			originY = 0;
			originSkewX = 0;
			originSkewY = 0;
			
			info = new BoneData();
			displays = { };
			
			children = new Vector.<Bone>;
			node = new Node();
			
			tween = new Tween(this);
			tweenNode = tween.node;
		}
		
		public function setOriginPosition(_x:Number, _y:Number, _skewX:Number = 0, _skewY:Number = 0):void {
			originX = _x;
			originY = _y;
			originSkewX = _skewX;
			originSkewY = _skewY;
		}
		
		public function recycleDisplay():void {
			if (__display) {
				displays[info.image] = __display;
				display = null;
			}
			info.hide = true;
		}
		
		public function cycleDisplay(_name:String = null):Boolean {
			if(_name){
				if(_name == info.image){
					if(info.hide){
					}else{
						return true;
					}
				}else{
					if(info.hide){
					}else{
						recycleDisplay();
					}
				}
			}else {
				if(info.image){
					if(info.hide){
						_name = info.image;
					}else{
						return true;
					}
				}else{
					return true;
				}
			}
			var _display:Object = displays[_name];
			if (_display) {
				delete displays[_name];
				display = _display;
				info.image = _name;
				info.hide = false;
				return true;
			}
			return false;
		}
		
		public function update():void {
			if (armature) {
				tween.update();
				
				var _transformX:Number = originX + node.x + tweenNode.x;
				var _transformY:Number = originY + node.y + tweenNode.y;
				var _transformSkewX:Number = originSkewX + node.skewX + tweenNode.skewX;
				var _transformSkewY:Number = originSkewY + node.skewY + tweenNode.skewY;
				if (parent.parent) {
					var _r:Number = Math.atan2(_transformY, _transformX) + parent.info.skewY;
					var _len:Number = Math.sqrt(_transformX * _transformX + _transformY * _transformY);
					_transformX = _len * Math.cos(_r) + parent.info.x;
					_transformY = _len * Math.sin(_r) + parent.info.y;
					_transformSkewX += parent.info.skewX;
					_transformSkewY += parent.info.skewY;
				}
				/*
				if(
					info.x != _transformX ||
					info.y != _transformY ||
					info.skewX != _transformSkewX ||
					info.skewY != _transformSkewY ||
					info.scaleX != tweenNode.scaleX ||
					info.scaleY != tweenNode.scaleY
				){*/
					info.x = _transformX;
					info.y = _transformY;
					info.skewX = _transformSkewX;
					info.skewY = _transformSkewY;
					info.scaleX = tweenNode.scaleX;
					info.scaleY = tweenNode.scaleY;
					if (__display) {
						armature.factory.updateDisplay(__display, info);
					}
				//}
			}
			
			for each(var _child:Bone in children) {
				_child.update();
			}
		}
		
		public function dispose():void{
			userData = null;
			factory = null;
			info = null;
			node = null;
			tween = null;
			armature = null;
			parent = null;
			children = null;
			tweenNode = null;
			__display = null;
		}
		
		internal function addChild(_child:Bone):void {
			if (!_child) {
				return;
			}
			var _parent:Bone = this;
			while (_parent) {
				if (_parent == _child) {
					return;
				}
				_parent = _parent.parent;
			}
			if (children.indexOf(_child) < 0) {
				if (_child.parent) {
					_child.parent.removeChild(_child);
				}
				_child.parent = this;
				_child.armature = (this as Armature) || armature;
				_child.info.parent = info.name;
				children.push(_child);
				if (armature) {
					armature.factory.displayContainer(_child.display, armature.display, info.z);
				}
			}
		}
		
		internal function removeChild(_child:Bone):void {
			if (!_child) {
				return;
			}
			var _index:int = children.indexOf(_child);
			if (_index >= 0) {
				_child.parent = null;
				children.splice(_index, 1);
				if (armature) {
					armature.factory.displayContainer(_child.display);
				}
			}
		}
	}
	
}