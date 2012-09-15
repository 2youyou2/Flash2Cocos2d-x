package akdcl.skeleton.objects {
	/**
	 * ...
	 * @author Akdcl
	 */
	internal class BaseDicData {
		public var name:String;
		protected var datas:Object;
		
		public function BaseDicData(_name:String = null) {
			name = _name;
			datas = { };
		}
		
		public function addData(_data:Object, _id:String = null):void {
			_id = _id || _data.name;
			if (datas[_id]) {
				datas[_id].dispose();
			}
			datas[_id] = _data;
		}
		
		public function getSearchList():Array {
			var _list:Array = [];
			for (var _name:String in datas) {
				_list.push(_name);
			}
			return _list;
		}
		
		public function dispose():void {
			name = null;
			datas = null;
		}
	}
	
}