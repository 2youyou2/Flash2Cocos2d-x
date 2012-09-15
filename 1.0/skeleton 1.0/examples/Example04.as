package 
{
	import effects.Shadow;
	import effects.ShadowContainer;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	
	import akdcl.skeleton.Armature;
	import akdcl.skeleton.ConnectionData;
	import akdcl.skeleton.BaseCommand;
	
	import akdcl.skeleton.export.ContourInstaller;
	import akdcl.skeleton.export.ConnectionEncoder;
	
	/**
	 * ...
	 * @author Akdcl
	 */
    [SWF(width="800", height="600", nodeRate="30", backgroundColor="#999999")]
	public class Example04 extends Sprite {
		private var armatures:Vector.<Armature>;
		
		public function Example04() {
			init();
		}
		
		private function init():void {
			var _mc:MovieClip = ContourInstaller.install(new Knight());
			ConnectionData.addData(ConnectionEncoder.encode(_mc));
			_mc.dispose();
			
			armatures = new Vector.<Armature>;
			armatures.push(addKinght("stand1", 200, 200));
			armatures.push(addKinght("stand2", 200, 400));
			armatures.push(addKinght("run1", 400, 200));
			armatures.push(addKinght("run2", 400, 400));
			armatures.push(addKinght("attack1", 600, 200));
			armatures.push(addKinght("attack2", 600, 400));
			
			addEventListener(Event.ENTER_FRAME, onEnterFrameHandler);
		}
		
		private function addKinght(_action:String, _x:Number, _y:Number):Armature {
			var _id:String = "knight";
			var _display:Sprite = new KnightJoints() as Sprite;
			_display.x = _x;
			_display.y = _y;
			addChild(_display);
			
			var _armature:Armature = BaseCommand.createArmature(_id, _id, _display);
			
			switch(_action) {
				case "stand1":
				case "stand2":
				case "run1":
				case "run2":
					_armature.animation.playTo(_action, 4, 20, true, 2);
					break;
				case "attack1":
					//注意这里因为攻击动画是多帧动画，ease使用了1，sineIn，这样每个关键帧的补间都体现了挥剑的迅速
					//ease是设定给每个关键帧直接过渡使用的，而不是整个动画的过渡，整个动画的缓动是线性的
					_armature.animation.playTo("attack", 4, 55, true, 1);
					//动画回调
					_armature.animation.onAnimation = animationHandler;
					break;
				case "attack2":
					//ease同上
					_armature.animation.playTo("attack", 4, 55, true, 1);
					Shadow.drawContainer = this;
					//为weapon添加粒子
					_armature.addBone("particle", "weapon", new ShadowContainer(), 0).setLockPosition(80, 0);
					break;
			}
			return _armature;
		}
		
		private function animationHandler(_aniType:String, _aniID:String, _frameID:String = null):void {
			trace(_aniType, _aniID, _frameID);
		}
		
		private function onEnterFrameHandler(e:Event):void {
			for each(var _armature:Armature in armatures) {
				_armature.update();
			}
		}
	}
}