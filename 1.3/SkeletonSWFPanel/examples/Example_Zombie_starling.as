package  {
	import flash.display.Sprite;
	import flash.utils.setTimeout;
	
	import akdcl.skeleton.utils.generateSkeletonData;
	import akdcl.skeleton.factorys.StarlingFactory;
	
	import starling.core.Starling;
	
    [SWF(width="800", height="600", frameRate="30", backgroundColor="#999999")]
	public class Example_Zombie_starling extends Sprite {
		[Embed(source = "./resources/Zombie.swf", mimeType = "application/octet-stream")]
		private static const ResourcesData:Class;
		
		public function Example_Zombie_starling() {
			StarlingFactory.lastInstance.skeletonData = generateSkeletonData(new ResourcesData());
			
			setTimeout(starlingInit, 100);
		}
		
		private function starlingInit():void {
			var _starling:Starling = new Starling(StarlingGame, stage);
			//_starling.antiAliasing = 1;
			_starling.showStats = true;
			_starling.start();
		}
	}
}

import flash.geom.Point;
import starling.display.Sprite;
import starling.events.EnterFrameEvent;
import starling.events.Touch;
import starling.events.TouchPhase;
import starling.events.TouchEvent;

import akdcl.skeleton.Armature;
import akdcl.skeleton.factorys.StarlingFactory;

class StarlingGame extends Sprite {
	private var allArmatureNameList:Array;
	private var armatures:Array;
	public function StarlingGame() {
		allArmatureNameList = StarlingFactory.lastInstance.skeletonData.getSearchList();
		armatures = [];
		
		addEventListener(EnterFrameEvent.ENTER_FRAME, onEnterFrameHandler);
	}
	
	private function onMouseClickHandler(_e:TouchEvent):void {
		var _touch:Touch = _e.getTouch(stage, TouchPhase.ENDED);
		if (!_touch) {
			return;
		}
		var _p:Point = _touch.getLocation(stage);
		
		var _randomID:String = allArmatureNameList[int(Math.random() * allArmatureNameList.length)];
		var _armature:Armature = StarlingFactory.lastInstance.buildArmature(_randomID);
		
		_armature.display.x = _p.x;
		_armature.display.y = _p.y;
		
		var _randomMovement:String = _armature.animation.movementList[int(Math.random() * _armature.animation.movementList.length)];
		_armature.animation.play(_randomMovement);
		addChild(_armature.display as Sprite);
		armatures.push(_armature);
	}
	
	private function onEnterFrameHandler(_e:EnterFrameEvent):void {
		if (stage && !stage.hasEventListener(TouchEvent.TOUCH)) {
			stage.addEventListener(TouchEvent.TOUCH, onMouseClickHandler);
		}
		for each(var _armature:Armature in armatures) {
			_armature.update();
		}
	}
}
