package 
{
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import akdcl.skeleton.Armature;
	import akdcl.skeleton.Bone;
	
	/**
	 * ...
	 * @author Akdcl
	 */
    [SWF(width="800", height="600", nodeRate="30", backgroundColor="#999999")]
	public class Example01 extends Sprite {
		protected var armature:Armature;
		protected var dragJoint:Sprite;
		protected var recordRotation:Number;
		
		private var clickPoint:Point = new Point();
		
		var speed:Number = 0.3; 
		var radius:Number = 0; 
		var angle:Number = 0; 
		var xpos:Number; 
		var ypos:Number; 

		var centerX:Number = stage.stageWidth / 2; 
		var centerY:Number = stage.stageHeight / 2; 

		 
		
		public function Example01() {
			init();
			
			//生成容器 
		var container:Sprite = new Sprite();

		//把容器加入舞台的中心 
		container.x = centerX; 
		container.y = centerY; 
		addChild (container); 

		//设定线条样式 4个像素、白色 
		container.graphics.lineStyle (4, 0xffffff); 

		//起始点在舞台的中心 
		container.graphics.moveTo (0, 0); 
		//注册进入帧事件侦听器  
		addEventListener (Event.ENTER_FRAME, onEnterFrame); 
			
		}
		
		//事件响应函数 
function onEnterFrame (event:Event):void { 

        /* We’ll stop the drawing after the radius is over 100  

pixels. 
        We still continue to rotate the container. 
        */ 
        if (radius > 100) { 
                speed = 0; 
                container.rotation += 10; 
        } 
        else { 
                //Increase the radius in each frame 
                radius += 0.5; 

                //New x and y coordinates 
                xpos = Math.cos(angle) * radius; 
                ypos = Math.sin(angle) * radius; 

                //Draw to the new coorninates 
                container.graphics.lineTo (xpos,ypos); 

                //Rotate the container 
                container.rotation += 10; 

                //The greater the speed, the faster we  draw circles 
                angle += speed; 
        } 
}
		
		private function init():void {
			addEventListener(Event.ENTER_FRAME, onEnterFrameHandler);
			addEventListener(MouseEvent.MOUSE_DOWN, onMouseHandler);
			stage.addEventListener(MouseEvent.MOUSE_UP, onMouseHandler);
			
			//显示对象，用来做骨架所有显示关节的容器
			var _sprite:Sprite = new Sprite();
			_sprite.x = 400;
			_sprite.y = 300;
			_sprite.buttonMode = true;
			addChild(_sprite);
			
			//骨架
			armature = new Armature(_sprite);
			
			//添加显示关节
			armature.addBone("body", null, createJoint(150, -75));
			
			armature.addBone("tail", "body", createJoint(50), 0).setLockPosition( -75, 0, -90);
			armature.addBone("legL1", "body", createJoint(), 1).setLockPosition( -75, 0, 60);
			armature.addBone("legL2", "body", createJoint(), 2).setLockPosition( -75, 0, 120);
			armature.addBone("legR1", "body", createJoint(), 3).setLockPosition(75, 0, 60);
			armature.addBone("legR2", "body", createJoint(), 4).setLockPosition(75, 0, 120);
			
			armature.addBone("neck", "body", createJoint(80), 5).setLockPosition(75, 0, -60);
			armature.addBone("head", "neck", createJoint(50), 6).setLockPosition(80, 0, 60);
		}
		
		//建立一个关节
		private function createJoint(_length:uint = 100, _offX:int=0, _color:uint=0x00ffff):Sprite {
			var _sprite:Sprite = new Sprite();
			var _graphics:Graphics = _sprite.graphics;
			_graphics.beginFill(_color, 0.3);
			_graphics.drawCircle(_length + _offX, 0, 16);
			_graphics.lineStyle(20, _color, 0.5);
			_graphics.moveTo(_offX, 0);
			_graphics.lineTo(_length + _offX, 0);
			return _sprite;
		}
		
		//驱动
		private function onEnterFrameHandler(e:Event):void {
			if (dragJoint) {
				//控制选中关节的rotation到一个合理的值
				var _dragBone:Bone = armature.getBone(dragJoint.name);
				_dragBone.node.rotation = 
					recordRotation +
					(
						Math.atan2(armature.getDisplay().mouseY - dragJoint.y, armature.getDisplay().mouseX - dragJoint.x) - 
						Math.atan2(clickPoint.y - dragJoint.y, clickPoint.x - dragJoint.x)
					) * 180 / Math.PI;
			}
			
			//驱动骨架
			armature.update();
		}
		
		//鼠标事件
		private function onMouseHandler(e:MouseEvent):void {
			if (e.type == MouseEvent.MOUSE_DOWN) {
				pressJoint(e.target as Sprite);
			}else if (dragJoint) {
				releaseJoint();
			}else {
				releaseOutSide();
			}
		}
		
		protected function pressJoint(_joint:Sprite):void {
			dragJoint = _joint;
			clickPoint.x = armature.getDisplay().mouseX;
			clickPoint.y = armature.getDisplay().mouseY;
			var _bone:Bone;
			//通过显示关节索引到对应骨骼bone，因显示关节的name与骨骼关系一致
			_bone = armature.getBone(dragJoint.name);
			recordRotation = _bone.node.rotation;
		}
		
		protected function releaseJoint():void {
			dragJoint = null;
		}
		
		protected function releaseOutSide():void {
			
		}
	}
}