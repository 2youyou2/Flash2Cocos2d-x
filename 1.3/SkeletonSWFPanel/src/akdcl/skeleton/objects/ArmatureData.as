package akdcl.skeleton.objects{
	
	/**
	 * ...
	 * @author Akdcl
	 */
	final public class ArmatureData extends BaseDicData {
		public function ArmatureData(_name:String = null) {
			super(_name);
		}
		
		override public function addData(_data:Object, _name:String = null):void {
			super.addData(_data, _name);
		}
		
		override public function getSearchList():Array{
			var _list:Array = super.getSearchList();
			_list.sort(sortBoneZIndex);
			return _list;
		}
		
		public function getData(_name:String):BoneData {
			return datas[_name];
		}
		
		private function sortBoneZIndex(_boneName1:String, _boneName2:String):int {
			return getData(_boneName1).z > getData(_boneName2).z?1: -1;
		}
	}
}