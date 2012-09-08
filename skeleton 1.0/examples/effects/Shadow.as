package effects{
	import flash.display.DisplayObjectContainer;
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.display.BlendMode;
	
	import flash.geom.Point;

	import flash.events.Event;

	public class Shadow extends Sprite {
		public static var drawContainer:DisplayObjectContainer;

		public var startX:Number = 0;
		public var startY:Number = 0;
		public var endX:Number = 20;
		public var endY:Number = 20;

		public var color:uint = 0x00ffff;
		public var alphaStart:Number = 1;
		public var shadowLength:uint = 10;
		public var tailLength:Number = 0;

		public var speed:Object;

		public var autoDraw:Boolean = true;

		public var shadowType:int;
		public var lineType:int = 1;

		public var isRemove:Boolean;

		protected var shadowList:Array;

		protected var fillSprite:Sprite;
		
		public function Shadow() {
			addEventListener(Event.ADDED_TO_STAGE, onAddToStageHandler);
		}
		
		private function onAddToStageHandler(e:Event):void 	{
			removeEventListener(Event.ADDED_TO_STAGE, onAddToStageHandler);
			if (!drawContainer){
				return;
			}
			shadowList = [];
			fillSprite = new Sprite();
			fillSprite.filters = filters;
			fillSprite.blendMode = blendMode;
			drawContainer.addChild(fillSprite);

			if (autoDraw){
				addEventListener(Event.ENTER_FRAME, runStep);
			}

			alphaStart = alpha;
			color = transform.colorTransform.color == 0 ? color : transform.colorTransform.color;

			visible = false;
			filters = null;
		}
		
		/*override protected function onRemoveHandler():void 
		{
			if (fillSprite && drawContainer.contains(fillSprite)){
				drawContainer.removeChild(fillSprite);
			}

			super.onRemoveHandler();
			speed = null;
			shadowList = null;
			fillSprite = null;
		}*/

		public function runStep(_evt:Event = null):void {
			if (isRemove){
				if (shadowList.length == 0){
					return;
				}
				shadowLength--;
				shadowList.pop();
				alphaStart -= drawShadow();
			} else {
				if (shadowList.length >= shadowLength){
					shadowList.pop();
				}

				shadowList.unshift(getPosNow());
				drawShadow();
			}
		}

		protected function getPosNow(){
			var _p1:Point = new Point(startX, startY);
			var _p2:Point = new Point(endX, endY);
			_p1 = fillSprite.globalToLocal(localToGlobal(_p1));
			//_p2 = fillSprite.globalToLocal(parent.localToGlobal(_p2));
			_p2 = fillSprite.globalToLocal(localToGlobal(_p2));
			return [_p1, _p2];
		}

		protected function drawShadow():Number {
			var _length:uint = shadowList.length;
			var _graphics:Graphics = fillSprite.graphics;
			if (_length > 1){
				_graphics.clear();
				var _alphaNow:Number = alphaStart;
				var _dAlpha:Number = _alphaNow / (shadowLength - 1);
				var _tailNow:Number = 1;
				var _dTail:Number = (1 - tailLength) / (shadowLength - 1);
				if (lineType > 0){
					_dTail *= 0.5;
				}

				var _p1:*;
				var _p2:*;
				var _p3:*;
				var _p4:*;
				var _p11:*;
				var _p22:*;
				var _p33:*;
				var _p44:*;

				for (var _i:int = 0; _i < _length - 1; _i++){
					_p1 = shadowList[_i][0];
					_p2 = shadowList[_i][1];
					_p3 = shadowList[_i + 1][1];
					_p4 = shadowList[_i + 1][0];
					//_p1.offset(speed.x,speed.y);
					//_p2.offset(speed.x,speed.y);

					if (speed){
						_p3.offset(speed.x, speed.y);
						_p4.offset(speed.x, speed.y);
					}
					if (_i == 0) {
						if (shadowType > 0){
							_p22 = Point.interpolate(_p2, _p1, _tailNow);
							_p11 = _p1;
						} else if (shadowType < 0){
							_p22 = _p2;
							_p11 = Point.interpolate(_p1, _p2, _tailNow);
						} else if (shadowType == 0){
							_p22 = Point.interpolate(_p2, _p1, _tailNow);
							_p11 = Point.interpolate(_p1, _p2, _tailNow);
						}
					}else {
						_p22 = _p33;
						_p11 = _p44;
					}
					
					if (shadowType > 0){
						_p33 = Point.interpolate(_p3, _p4, _tailNow);
						_p44 = _p4;
					} else if (shadowType < 0){
						_p33 = _p3;
						_p44 = Point.interpolate(_p4, _p3, _tailNow);
					} else if (shadowType == 0){
						_p33 = Point.interpolate(_p3, _p4, _tailNow);
						_p44 = Point.interpolate(_p4, _p3, _tailNow);
					}

					_graphics.beginFill(color, _alphaNow);
					_graphics.moveTo(_p11.x, _p11.y);
					_graphics.lineTo(_p22.x, _p22.y);
					_graphics.lineTo(_p33.x, _p33.y);
					_graphics.lineTo(_p44.x, _p44.y);
					_graphics.endFill();

					_alphaNow -= _dAlpha;
					_tailNow -= _dTail;
				}
				return _dAlpha;
			}
			return 0;
		}
	}
}