package {
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.Bitmap;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import akdcl.skeleton.Armature;
	import akdcl.skeleton.ConnectionData;
	import akdcl.skeleton.BaseCommand;
	
	import akdcl.skeleton.export.ContourInstaller;
	import akdcl.skeleton.export.ConnectionEncoder;
	import akdcl.skeleton.export.TexturePacker;
	
	//starling
	import starling.core.Starling;
	
	/**
	 * ...
	 * @author Akdcl
	 */
    [SWF(width="800", height="600", nodeRate="30", backgroundColor="#999999")]
	public class Example03 extends Sprite {
		private var armature:Armature;
		
		public function Example03() {
			init();
		}
		
		private function init():void {
			var _mc:MovieClip = ContourInstaller.install(new Knight());
			var _xml:XML = ConnectionEncoder.encode(_mc);
			trace(_xml);
			
			/*
			把xml数据再转成内置数据
			*/
			ConnectionData.addData(_xml);
			
			//为starling生成贴图
			var _packer:TexturePacker = new TexturePacker();
			_packer.addTexturesFromContainer(_mc, _mc.getName());
			StarlingGame.texture = _packer.packTextures(128, 2);
			_packer.dispose();
			_mc.dispose();
			
			//看看贴图的样子
			addChild(new Bitmap(StarlingGame.texture.bitmapData)).y = 20;
			
			//普通stage动画
			var _id:String = "knight";
			armature = BaseCommand.createArmature(_id, _id, new KnightJoints());
			armature.getDisplay().x = 300;
			armature.getDisplay().y = 300;
			
			addChild(armature.getDisplay() as Sprite);
			armature.animation.playTo("run", 4, 20, true, 2);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrameHandler);
			stage.addEventListener(MouseEvent.MOUSE_WHEEL, onMouseWheelHandler);
			
			//starling动画
			var _starling:Starling = new Starling(StarlingGame, stage);
			_starling.antiAliasing = 1;
			_starling.showStats = true;
			_starling.start();
		}
		
		private function onEnterFrameHandler(_e:Event):void {
			armature.update();
		}
		
		private function onMouseWheelHandler(_e:MouseEvent):void {
			if (_e.delta < 0) {
				if (armature.animation.scale > 0.2) {
					armature.animation.scale-= 0.1;
					armature.animation.setAnimationScale(armature.animation.scale);
				}
				
				//starling
				if (StarlingGame.instance.armature.animation.scale > 0.2) {
					StarlingGame.instance.armature.animation.scale-= 0.1;
					StarlingGame.instance.armature.animation.setAnimationScale(armature.animation.scale);
				}
			}else {
				if (armature.animation.scale < 4) {
					armature.animation.scale += 0.1;
					armature.animation.setAnimationScale(armature.animation.scale);
				}
				
				//starling
				if (StarlingGame.instance.armature.animation.scale < 4) {
					StarlingGame.instance.armature.animation.scale += 0.1;
					StarlingGame.instance.armature.animation.setAnimationScale(StarlingGame.instance.armature.animation.scale);
				}
			}
		}
	}
}

//starling
import starling.display.Sprite;
import starling.events.EnterFrameEvent;

import akdcl.skeleton.Armature;
import akdcl.skeleton.StarlingCommand;

import akdcl.skeleton.export.TextureMix;

class StarlingGame extends Sprite {
	public static var texture:TextureMix;
	public static var instance:StarlingGame;
	
	public var armature:Armature;
	public function StarlingGame() {
		instance = this;
		
		var _id:String = "knight";
		armature = StarlingCommand.createArmature(_id, _id, texture);
		armature.getDisplay().x = 500;
		armature.getDisplay().y = 300;
		addChild(armature.getDisplay() as Sprite);
		armature.animation.playTo("run", 4, 20, true, 2);
		
		addEventListener(EnterFrameEvent.ENTER_FRAME, onEnterFrameHandler);
	}
	
	private function onEnterFrameHandler(_e:EnterFrameEvent):void {
		armature.update();
	}
}