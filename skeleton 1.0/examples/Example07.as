package 
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.Bitmap;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	
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
	public class Example07 extends Sprite {
		public function Example07() {
			init();
		}
		
		private function init():void {
			var _mc:MovieClip = ContourInstaller.install(new RobotBiped());
			ConnectionData.addData(ConnectionEncoder.encode(_mc));
			
			var _packer:TexturePacker = new TexturePacker();
			_packer.addTexturesFromContainer(_mc, _mc.getName());
			StarlingGame.texture = _packer.packTextures(256, 2);
			_packer.dispose();
			_mc.dispose();
			
			//starling
			var _starling:Starling = new Starling(StarlingGame, stage);
			_starling.antiAliasing = 1;
			_starling.showStats = true;
			_starling.start();
			
			stage.addEventListener(KeyboardEvent.KEY_DOWN, onKeyEventHandler);
			stage.addEventListener(KeyboardEvent.KEY_UP, onKeyEventHandler);
		}

		private var left:Boolean;
		private var right:Boolean;
		
		private function onKeyEventHandler(e:KeyboardEvent):void {
			switch (e.keyCode) {
				case 37 :
				case 65 :
					left = e.type == KeyboardEvent.KEY_DOWN;
					updateMove(-1);
					break;
				case 39 :
				case 68 :
					right = e.type == KeyboardEvent.KEY_DOWN;
					updateMove(1);
					break;
				case 38 :
				case 87 :
					if (e.type == KeyboardEvent.KEY_DOWN) {
						StarlingGame.instance.jump();
					}
					break;
				case 83 :
				case 40 :
					StarlingGame.instance.squat(e.type == KeyboardEvent.KEY_DOWN);
					break;
			}
		}
		
		private function updateMove(_dir:int):void {
			if (left && right) {
				StarlingGame.instance.move(_dir);
			}else if (left){
				StarlingGame.instance.move(-1);
			}else if (right){
				StarlingGame.instance.move(1);
			}else {
				StarlingGame.instance.move(0);
			}
		}
	}
}

import flash.geom.Point;
import starling.display.Sprite;
import starling.events.EnterFrameEvent;
import starling.events.Touch;
import starling.events.TouchEvent;

import akdcl.skeleton.Armature;
import akdcl.skeleton.Bone;
import akdcl.skeleton.StarlingCommand;

import akdcl.skeleton.export.TextureMix;

class StarlingGame extends Sprite {
	public static var texture:TextureMix;
	public static var instance:StarlingGame;
	
	public function StarlingGame() {
		instance = this;
		
		var _id:String = "robotBiped";
		armature = StarlingCommand.createArmature(_id, _id, texture);
		armatureClip = armature.getDisplay() as Sprite;
		armatureClip.x = 400;
		armatureClip.y = 500;
		addChild(armatureClip);
		
		jump();
		
		addEventListener(EnterFrameEvent.ENTER_FRAME, onEnterFrameHandler);
	}
	
	private var armature:Armature;
	private var armatureClip:Sprite;
	private var mouseX:Number = 0;
	private var mouseY:Number = 0;
	private var isJumping:Boolean;
	private var isSquat:Boolean;
	private var moveDir:int;
	private var face:int;
	
	private var speedX:Number = 0;
	private var speedY:Number = 0;
	
	public function move(_dir:int):void {
		if (moveDir == _dir) {
			return;
		}
		moveDir = _dir;
		updateMovement();
	}
	
	public function jump():void {
		if (isJumping) {
			return;
		}
		speedY = -20;
		isJumping = true;
		armature.animation.playTo("jump", 8);
	}
	
	public function squat(_isDown:Boolean):void {
		if (isSquat == _isDown) {
			return;
		}
		isSquat = _isDown;
		updateMovement();
	}
	
	private function updateMovement():void {
		if (isJumping) {
			return;
		}
		if (isSquat) {
			speedX = 0;
			armature.animation.playTo("squat", 8);
			return;
		}
		
		if (moveDir == 0) {
			speedX = 0;
			armature.animation.playTo("stand", 8, 40, true, 2);
		}else {
			if (moveDir * face > 0) {
				speedX = 8* face;
				armature.animation.playTo("run", 8, 24, true, 2);
			}else {
				speedX = -5 * face;
				armature.animation.playTo("runBack", 4, 32, true, 2);
			}
		}
	}
	
	private function updateSpeed():void {
		if (isJumping) {
			if (speedY <= 0 && speedY + 1 > 0 ) {
				armature.animation.playTo("fall", 8);
			}
			speedY += 1;
		}
		if (speedX != 0) {
			armatureClip.x += speedX;
			if (armatureClip.x < 0) {
				armatureClip.x = 0;
			}else if (armatureClip.x > 800) {
				armatureClip.x = 800;
			}
		}
		if (speedY != 0) {
			armatureClip.y += speedY;
			if (armatureClip.y > 500) {
				armatureClip.y = 500;
				isJumping = false;
				speedY = 0;
				
				updateMovement();
			}
		}
	}
	
	private function updateWeapon():void {
		face = mouseX > armatureClip.x?1: -1;
		if (armatureClip.scaleX != face) {
			armatureClip.scaleX = face;
			updateMovement();
		}
	
		var _r:Number;
		if(face>0){
			_r = Math.atan2(mouseY - armatureClip.y, mouseX - armatureClip.x) * 180 / Math.PI;
		}else{
			_r = 180 - Math.atan2(mouseY - armatureClip.y, mouseX - armatureClip.x) * 180 / Math.PI;
			if (_r > 180) {
				_r -= 360;
			}
		}
		
		var _body:Bone = armature.getBone("body");
		_body.node.rotation=_r*0.25;
		
		var _chest:Bone = armature.getBone("chest");
		_chest.node.rotation=_r*0.25;
		
		var _head:Bone = armature.getBone("head");
		if(_r>0){
			_head.node.rotation=_r*0.2;
		}else{
			_head.node.rotation=_r*0.4;
		}
		
		var _armR:Bone = armature.getBone("upperarmR");
		var _armL:Bone = armature.getBone("upperarmL");
		_armR.node.rotation = _r*0.5;
		if(_r>0){
			_armL.node.rotation=_r*0.8;
		}else{
			_armL.node.rotation=_r*0.6;
		}
	}
	
	private function onTouchHandler(_e:TouchEvent):void {
		var _p:Point = _e.getTouch(stage).getLocation(stage);
		mouseX = _p.x; 
		mouseY = _p.y; 
	}
	
	private function onEnterFrameHandler(_e:EnterFrameEvent):void {
		if (stage && !stage.hasEventListener(TouchEvent.TOUCH)) {
			stage.addEventListener(TouchEvent.TOUCH, onTouchHandler);
		}
		
		updateSpeed();
		updateWeapon();
		armature.update();
	}
}