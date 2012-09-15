package akdcl.skeleton {
	
	/**
	 * 基本骨架生成工具
	 * 从 ConnectionData 的数据中生成骨架
	 * @author Akdcl
	 */
	final public class BaseCommand {
		
		/**
		 * Armature 工厂接口，默认生成 Armature 实例，如果需要生成特殊的 Armature 子类，可以利用这个接口
		 * @param _name 骨架名
		 * @param _animationName 动画名
		 * @param _armatureDisplay 骨架显示对象
		 */
		public static var armatureFactory:Function;
		
		/**
		 * 骨架生成方法
		 * @param _name 骨架名
		 * @param _animationName 动画名
		 * @param _armarureDisplayFactory 骨架显示对象的实例或是一个方法，通过该方法返回骨架显示对象的实例，传入参数为骨架名
		 * @param _boneDisplayFactory 骨骼显示对象的构建方法，如果不传递，在 stage 中将根据骨骼名，在骨架的显示对象中查询是否有符合这个名字的显示对象
		 * @param _isRadian 骨骼旋转角度是否采用弧度制，比如 Starling 使用的是弧度制
		 * @param _useLocalXYZ 是否使用骨骼现有的相对位置数据而不是 ConnectionData 中的
		 */
		public static function createArmature(_name:String, _animationName:String, _armarureDisplayFactory:* = null, _boneDisplayFactory:Function = null, _isRadian:Boolean = false, _useLocalXYZ:Boolean = false):Armature {
			var _armatureData:XMLList = ConnectionData.getArmatureData(_name);
			if(!_armatureData){
				return null;
			}
			var _armatureDisplay:Object;
			
			if (_armarureDisplayFactory != null) {
				if (_armarureDisplayFactory is Function) {
					_armatureDisplay = _armarureDisplayFactory(_name);
				}else {
					_armatureDisplay = _armarureDisplayFactory;
				}
			}
			if (!_armatureDisplay) {
				return null;
			}
			try{
				_armatureDisplay.name = _name;
			}catch (e:Error) {
			}
			
			var _armature:Armature;
			if (armatureFactory != null) {
				_armature = armatureFactory(_name, _animationName, _armatureDisplay) as Armature;
			}else {
				_armature = new Armature(_armatureDisplay, _isRadian);
			}
			
			var _animationData:* = ConnectionData.getAnimationData(_animationName);
			if (_animationData) {
				_armature.animation.setData(_animationData);
			}
			
			var _bone:Bone;
			var _boneData:XML;
			var _boneName:String;
			var _parentName:String;
			var _boneDisplay:Object;
			var _displayHigher:Object;
			var _indexZ:int;
			var _list:Array = [];
			var _length:uint = _armatureData.length();
			//trace(_list);
			for(var indexI:uint = 0; indexI < _length; indexI++){
				_boneData = _armatureData[indexI];
				//trace(_boneData);
				_boneName = String(_boneData.@name);
				_parentName = String(_boneData.@parent);
				_indexZ = _useLocalXYZ? -2:int(_boneData.@z);
				
				
				if (_boneDisplayFactory != null) {
					_boneDisplay = _boneDisplayFactory(_name, _boneName);
				}else {
					_boneDisplay = _armature.getDisplay().getChildByName(_boneName);
				}
				
				if (!_useLocalXYZ && _boneDisplay) {
					_displayHigher = null;
					
					for(var indexJ:uint = _indexZ; indexJ < _list.length; indexJ++){
						_displayHigher = _list[indexJ];
						if(_displayHigher){
							break;
						}
					}
					_list[_indexZ] = _boneDisplay;
					if(_displayHigher){
						
						_indexZ = _armature.getDisplay().getChildIndex(_displayHigher);
					}else{
						_indexZ = -1;
					}
				}
				//trace(_boneDisplay.name + " " + _indexZ);
				_bone = _armature.addBone(_boneName, _parentName, _boneDisplay, _indexZ);
				_bone.setLockPosition(Number(_boneData.@x), Number(_boneData.@y), 0);
			}
			return _armature;
		}
	}
}