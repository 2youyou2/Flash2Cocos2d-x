package akdcl.skeleton.export{
	import flash.display.MovieClip;
	
	/**
	 * ContourInstaller
	 * @author Akdcl
	 */
	final public class ContourInstaller {
		public static function isInstalled(_mc:MovieClip):Boolean {
			return _mc.installed;
		}
		public static function install(_mc:MovieClip):MovieClip {
			if (_mc) {
				if (("xml" in _mc)?_mc.installed:true) {
					return null;
				}
			}else {
				return null;
			}
			
			_mc.getName = function():String {
				if (this.xml) {
					return this.xml.name();
				}
				return null;
			}
			
			_mc.setValue = function(_id:String, _key:String, _v:*):void {
				var _value:Object = this.values[_id];
				if (!_value) {
					_value = this.values[_id] = { };
				}
				_value[_key] = _v;
			}
			
			_mc.getValue = function(_id:String, _key:String):* {
				var _value:Object = this.values[_id];
				if (_value) {
					return _value[_key];
				}
				return false;
			}
			
			_mc.clearValues = function():void {
				this.values = { };
			}
			
			_mc.reset = function():void {
				this.clearValues();
				this.gotoAndStop(1);
			}
			
			_mc.dispose = function():void {
				this.values = null;
				this.xml = null;
				this.stop();
				if (this.parent) {
					this.parent.removeChild(this);
				}
			}
			
			_mc.installed = true;
			_mc.reset();
			return _mc;
		}
	}
	
}