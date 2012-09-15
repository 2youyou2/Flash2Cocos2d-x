package  {
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.setTimeout;
	
	import akdcl.skeleton.Armature;
	import akdcl.skeleton.utils.generateSkeletonData;
	import akdcl.skeleton.factorys.BaseFactory;
	
    [SWF(width="800", height="600", frameRate="30", backgroundColor="#999999")]
	public class Example_Zombie_normal extends Sprite {
		[Embed(source = "./resources/Zombie.swf", mimeType = "application/octet-stream")]
		private static const ResourcesData:Class;
		
		private var allArmatureNameList:Array;
		
		private var armatures:Array;
		
		public function Example_Zombie_normal() {
			BaseFactory.lastInstance.skeletonData = generateSkeletonData(new ResourcesData());
			allArmatureNameList = BaseFactory.lastInstance.skeletonData.getSearchList();
			armatures = [];
			setTimeout(baseInit, 100);
		}
		
		private function baseInit():void {
			stage.addEventListener(MouseEvent.CLICK, onMouseClickHandler);
			addEventListener(Event.ENTER_FRAME, onEnterFrameHandler);
		}
		
		private function onMouseClickHandler(_e:Event):void {
			var _randomID:String = allArmatureNameList[int(Math.random() * allArmatureNameList.length)];
			var _armature:Armature = BaseFactory.lastInstance.buildArmature(_randomID);
			
			_armature.display.x = mouseX;
			_armature.display.y = mouseY;
			
			var _randomMovement:String = _armature.animation.movementList[int(Math.random() * _armature.animation.movementList.length)];
			_armature.animation.play(_randomMovement);
			addChild(_armature.display as Sprite);
			armatures.push(_armature);
		}
		
		private function onEnterFrameHandler(_e:Event):void {
			for each(var _armature:Armature in armatures) {
				_armature.update();
			}
		}
	}
}
