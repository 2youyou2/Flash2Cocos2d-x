package 
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.Bitmap;
	import flash.events.Event;
	
	import akdcl.skeleton.ConnectionData;
	
	import akdcl.skeleton.export.ContourInstaller;
	import akdcl.skeleton.export.ConnectionEncoder;
	import akdcl.skeleton.export.TexturePacker;
	
	import starling.core.Starling;
	
	/**
	 * ...
	 * @author Akdcl
	 */
    [SWF(width="800", height="600", nodeRate="30", backgroundColor="#999999")]
	public class Example05 extends Sprite {
		public function Example05() {
			init();
		}
		
		private function init():void {
			var _mc:MovieClip = ContourInstaller.install(new Knight());
			ConnectionData.addData(ConnectionEncoder.encode(_mc));
			
			//生成贴图数据
			
			var _packer:TexturePacker = new TexturePacker();
			_packer.addTexturesFromContainer(_mc, _mc.getName());
			StarlingGame.texture = _packer.packTextures(128, 2);
			_packer.dispose();
			_mc.dispose();
			
			//看看贴图的样子
			addChild(new Bitmap(StarlingGame.texture.bitmapData)).y = 20;
			
			//starling
			var _starling:Starling = new Starling(StarlingGame, stage);
			_starling.antiAliasing = 1;
			_starling.showStats = true;
			_starling.start();
		}
	}
}

import starling.display.Sprite;
import starling.events.EnterFrameEvent;

import akdcl.skeleton.Armature;
import akdcl.skeleton.Animation;
import akdcl.skeleton.BaseCommand;
import akdcl.skeleton.StarlingCommand;

import akdcl.skeleton.export.TextureMix;

class StarlingGame extends Sprite {
	public static var texture:TextureMix;
	public static var instance:StarlingGame;
	
	private var armatures:Vector.<KnightArmature>;
	public function StarlingGame() {
		instance = this;
		BaseCommand.armatureFactory = armatureFactory;
		
		var _id:String = "knight";
		armatures = new Vector.<KnightArmature>;
		var _knightArmature:KnightArmature;
		for (var _i:uint = 0; _i < 200; _i++ ) {
			_knightArmature = StarlingCommand.createArmature(_id, _id, texture) as KnightArmature;
			_knightArmature.randomRun();
			addChild(_knightArmature.getDisplay() as Sprite);
			armatures.push(_knightArmature);
		}
		
		addEventListener(EnterFrameEvent.ENTER_FRAME, onEnterFrameHandler);
	}
	
	private function armatureFactory(_name:String, _aniName:String, _display:Object):Armature {
		return new KnightArmature(_display);
	}
	
	private function onEnterFrameHandler(_e:EnterFrameEvent):void {
		armatures.sort(sortDepth);
		for each(var _armature:KnightArmature in armatures) {
			addChild(_armature.getDisplay() as Sprite);
			_armature.update();
		}
	}
	
	private function sortDepth(_a1:KnightArmature, _a2:KnightArmature):int {
		return _a1.depth > _a2.depth?1: -1;
	}
}

class KnightArmature extends Armature {
	private var speedX:Number;
	private var speedY:Number;
	private var face:int;
	
	public function get depth():Number {
		return display.y;
	}
	
	public function KnightArmature(_display:Object) {
		super(_display,true);
		face = Math.random() > 0.5?1: -1;
		
		display.x = -100 * Math.random() - 100;
		display.y = 200 + Math.random() * 300;
		display.scaleX = face;
		
		animation.onAnimation = animationHandler;
	}
	
	public function randomRun():void {
		var _scale:Number = Math.random() * 0.3 + 0.7;
		speedX = _scale * 5;
		speedY = Math.random() * 2;
		animation.setAnimationScale(_scale);
		animation.playTo("run", 10, 20, true, 2);
	}
	
	override public function update():void {
		super.update();
		
		display.x += speedX * face;
		display.y += speedY;
		if (face > 0) {
			if (display.x > StarlingGame.instance.stage.stageWidth + 100) {
				display.x = -100;
			}
		}else {
			if (display.x < 0 - 100) {
				display.x = StarlingGame.instance.stage.stageWidth + 100;
			}
		}
		
		if (display.y < 200) {
			display.y = 200;
			speedY = Math.random() * 2;
		}else if (display.y > StarlingGame.instance.stage.stageHeight) {
			speedY = -Math.random() * 2;
		}
	}
	
	private function animationHandler(_aniType:String, _aniID:String, _frameID:String = null):void {
		switch(_aniType) {
			case Animation.LOOP_COMPLETE:
				switch(_aniID) {
					case "run":
						if (Math.random() > 0.90) {
							speedX = 0;
							speedY = 0;
							if (Math.random()>0.5) {
								animation.playTo("stand", 4, 20, true, 2);
							}else {
								animation.playTo("attack", 4, 50, false, 1);
							}
						}
						break;
					case "stand":
						if (Math.random() > 0.60) {
							randomRun();
						}
						break;
				}
				break;
			case Animation.COMPLETE:
				randomRun();
				break;
		}
	}
}