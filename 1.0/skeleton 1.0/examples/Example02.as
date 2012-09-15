package 
{
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import akdcl.skeleton.Bone;
	import akdcl.skeleton.FrameNode;
	import akdcl.skeleton.FrameNodeList;
	
	/**
	 * ...
	 * @author Akdcl
	 */
    [SWF(width="800", height="600", nodeRate="30", backgroundColor="#999999")]
	public class Example02 extends Example01 {
		
		override protected function pressJoint(_joint:Sprite):void {
			super.pressJoint(_joint);
			//暂停整个骨架动画
			armature.animation.pause();
		}
		
		override protected function releaseJoint():void {
			//继续整个骨架的动画
			armature.animation.resume();
			
			//实现点击关节骨骼播放列表动画
			var _bone:Bone = armature.getBone(dragJoint.name);
			var _node:FrameNode;
			
			//生成列表动画
			var _nodeList:FrameNodeList = new FrameNodeList();
			
			_node = new FrameNode(0, 0, Math.random() * 20);
			_nodeList.addFrame(_node);
			_node = new FrameNode(0, 0, -Math.random() * 20);
			_nodeList.addFrame(_node);
			
			//播放列表动画
			armature.animation.getTween(dragJoint.name).playTo(_nodeList, 15, 30, true, 2);
			super.releaseJoint();
		}
		
		override protected function releaseOutSide():void 
		{
			super.releaseOutSide();
			//停止动画
			armature.animation.pause();
		}
	}
}