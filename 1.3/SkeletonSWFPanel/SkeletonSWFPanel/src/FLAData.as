package{
	import adobe.utils.MMExecute;
	
	import com.grapefrukt.exporter.simple.SimpleExport;
	import com.grapefrukt.exporter.extractors.*;
	
	import flash.utils.getDefinitionByName;
	import flash.utils.getQualifiedClassName;
	
	import akdcl.skeleton.Armature;
	import akdcl.skeleton.factorys.BaseFactory;
	import akdcl.skeleton.objects.AnimationData;
	import akdcl.skeleton.objects.MovementBoneData;
	import akdcl.skeleton.objects.MovementData;
	import akdcl.skeleton.objects.SkeletonData;
	import akdcl.skeleton.utils.ConstValues;
	import akdcl.skeleton.utils.extractXML;
	import akdcl.skeleton.utils.generateArmatureData;
	import akdcl.skeleton.utils.generateMovementBoneData;
	import akdcl.skeleton.utils.generateMovementData;
	import akdcl.skeleton.utils.generateSkeletonData;
	
	import flash.display.DisplayObject;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.MouseEvent;
	import flash.net.FileFilter;
	import flash.net.FileReference;
	import flash.net.URLLoader;
	import flash.net.URLLoaderDataFormat;
	import flash.net.URLRequest;
	import flash.utils.ByteArray;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import mx.collections.ArrayCollection;
	import mx.collections.XMLListCollection;
	import mx.controls.Alert;
	import mx.managers.PopUpManager;
	
	[Bindable]
	public final class FLAData extends EventDispatcher{
		public static const AT_NAME:String = ConstValues.AT + ConstValues.A_NAME;
		
		public static const SOURCE_UPDATE:String = "sourceUpdate";
		public static const ARMATURE_UPDATE:String = "armatureUpdate";
		public static const MOVEMENT_UPDATE:String = "movementUpdate";
		public static const BONE_UPDATE:String = "boneUpdate";
		
		private static const FILE_FILTER_ARRAY:Array = [new FileFilter("Exported data", "*." + String(["swf", "png"]).replace(/\,/g, ";*."))];
		
		private static var __instance:FLAData;
		public static function get instance():FLAData{
			if(!__instance){
				__instance = new FLAData();
			}
			return __instance;
		}
		
		public var export:SimpleExport;
		
		public var xml:XML;
		private var armaturesXML:XML;
		private var animationsXML:XML;
		private var textureAtlasXML:XML;
		
		private var armatureXML:XML;
		private var animationXML:XML;
		private var movementXML:XML;
		private var movementBoneXML:XML;
		
		private var urlLoader:URLLoader;
		private var fileREF:FileReference;
		private var isTextureChanged:Boolean;
		private var isExporting:Boolean;
		private var setTimeoutID:int;
		private var saveList:Array;
		
		private var skeletonData:SkeletonData;
		private var armatures:Object;
		private var armature:Armature;
		private var shape:Shape;
		private var alert:Alert;
		private var isDraged:Boolean;
		
		private var maxWidth:int;
		private var maxHeight:int;
		
		public var armaturesMC:XMLListCollection;
		public var movementsMC:XMLListCollection;
		public var bonesMC:XMLListCollection;
		public var texturesMC:XMLListCollection;
		
		public var container:Sprite;
		public var textureDisplay:DisplayObject;
		
		public var isSWFSource:Boolean;
		
		public var dataImportID:int = 0;
		public var dataImportAC:ArrayCollection = new ArrayCollection(["All library items", "Seleted items", "Exported SWF/PNG"]);
		
		public var dataExportID:int = 0;
		public var dataExportAC:ArrayCollection = new ArrayCollection(["PNG", "SWF", "JSON", "Single PNG and XML"]);
		
		public var textureMaxWidthID:int = 2;
		public var textureMaxWidthAC:ArrayCollection = new ArrayCollection([128, 256, 512, 1024, 2048, 4096]);
		
		public var textureInterval:int = 2;
		
		public var textureSortID:int = 0;
		public var textureSortAC:ArrayCollection = new ArrayCollection(["MaxRects"]);
		
		public function get durationTo():int{
			if(!movementXML){
				return -1;
			}
			return int(movementXML.attribute(ConstValues.A_DURATION_TO));
		}
		public function set durationTo(_value:int):void{
			if(movementXML){
				movementXML[ConstValues.AT + ConstValues.A_DURATION_TO] = _value;
				changeMovement();
			}
		}
		
		public function get durationTween():int{
			if(movementXML?int(movementXML.attribute(ConstValues.A_DURATION)) == 1:true){
				return -1;
			}
			return int(movementXML.attribute(ConstValues.A_DURATION_TWEEN));
		}
		public function set durationTween(_value:int):void{
			if(movementXML){
				movementXML[ConstValues.AT + ConstValues.A_DURATION_TWEEN] = _value;
				changeMovement();
			}
		}
		
		public function get loop():Boolean{
			return movementXML?Boolean(int(movementXML.attribute(ConstValues.A_LOOP))):false;
		}
		public function set loop(_value:Boolean):void{
			if(movementXML){
				movementXML[ConstValues.AT + ConstValues.A_LOOP] = _value?1:0;
				changeMovement();
			}
		}
		
		public function get tweenEasing():Number{
			return movementXML?Number(movementXML.attribute(ConstValues.A_TWEEN_EASING)):0;
		}
		public function set tweenEasing(_value:Number):void{
			if(movementXML){
				movementXML[ConstValues.AT + ConstValues.A_TWEEN_EASING] = _value;
				changeMovement();
			}
		}
		
		public function get boneScale():int{
			return (movementBoneXML?Number(movementBoneXML.attribute(ConstValues.A_MOVEMENT_SCALE)):1) * 100;
		}
		public function set boneScale(_value:int):void{
			if(movementBoneXML){
				movementBoneXML[ConstValues.AT + ConstValues.A_MOVEMENT_SCALE] = _value * 0.01;
				changeMovementBone();
			}
		}
		
		public function get boneDelay():int{
			return (movementBoneXML?Number(movementBoneXML.attribute(ConstValues.A_MOVEMENT_DELAY)):0) * 100;
		}
		public function set boneDelay(_value:int):void{
			if(movementBoneXML){
				movementBoneXML[ConstValues.AT + ConstValues.A_MOVEMENT_DELAY] = _value * 0.01;
				changeMovementBone();
			}
		}
		
		public function get stageFrameRate():int{
			return container.stage.frameRate;
		}
		public function set stageFrameRate(_value:int):void{
			if(container){
				container.stage.frameRate = _value;
			}
		}
		
		public function FLAData(){
			super(this);
			
			export = new SimpleExport(null);
			
			armaturesMC = new XMLListCollection();
			movementsMC = new XMLListCollection();
			bonesMC = new XMLListCollection();
			texturesMC = new XMLListCollection();
			
			fileREF = new FileReference();
			
			urlLoader = new URLLoader();
			if(JSFL.isAvailable){
				urlLoader.addEventListener(Event.COMPLETE, onJSFLLoadedHandler);
				urlLoader.addEventListener(IOErrorEvent.IO_ERROR, onJSFLLoadedHandler);
				urlLoader.load(new URLRequest(JSFL.JSFL_URL));
			}
			
			container = new Sprite();
			shape = new Shape();
			shape.graphics.drawRect(0,0,100,100);
			container.addChild(shape);
			container.addEventListener(Event.ENTER_FRAME, onUpdateDisplayHandler);
			container.addEventListener(MouseEvent.MOUSE_DOWN, onContainerMouseHandler);
			container.addEventListener(MouseEvent.MOUSE_UP, onContainerMouseHandler);
			
		}
		
		private function onUpdateDisplayHandler(_e:Event):void{
			var _sW:int;
			var _sH:int;
			var _display:Object;
			if(armature){
				armature.update();
				_display = armature.display;
				if(_display){
					_sW = shape.width;
					_sH = shape.height;
					maxWidth = Math.max(_display.width, maxWidth);
					maxHeight = Math.max(_display.height, maxHeight);
					if(_sW < maxWidth || _sH < maxHeight){
						_sW = shape.width = maxWidth * 1.2;
						_sH = shape.height = maxHeight * 1.2;
					}else if(_sW > maxWidth * 1.5 || _sH > maxHeight * 1.5){
						_sW = shape.width = maxWidth * 1.2;
						_sH = shape.height = maxHeight * 1.2;
					}
					if(!isDraged){
						_display.x = _sW * 0.5;
						_display.y = _sH * 0.5;
					}
				}
				
			}
		}
		
		private function onContainerMouseHandler(_e:Event):void{
			var _display:Object = armature?armature.display:null;
			if(!_display){
				return;
			}
			switch(_e.type){
				case MouseEvent.MOUSE_DOWN:
					isDraged = true;
					_display.startDrag();
					break;
				case MouseEvent.MOUSE_UP:
					_display.stopDrag();
					break;
			}
		}
		
		private function onJSFLLoadedHandler(_e:Event):void{
			urlLoader.removeEventListener(Event.COMPLETE, onJSFLLoadedHandler);
			urlLoader.removeEventListener(IOErrorEvent.IO_ERROR, onJSFLLoadedHandler);
			switch(_e.type){
				case Event.COMPLETE:
					JSFL.skeletonJSFL = _e.target.data;
					break;
				case IOErrorEvent.IO_ERROR:
					break;
			}
		}
		
		public function importData():void{
			switch(dataImportID){
				case 0:
					if(JSFL.isAvailable){
						generateFLAXML(JSFL.getArmatureList());
					}
					break;
				case 1:
					if(JSFL.isAvailable){
						generateFLAXML(JSFL.getArmatureList(true));
					}
					break;
				case 2:
					fileREF.addEventListener(Event.SELECT, onFileLoadHaneler);
					fileREF.browse(FILE_FILTER_ARRAY);
					break;
			}
		}
		
		private function onFileLoadHaneler(_e:Event):void{
			switch(_e.type){
				case Event.SELECT:
					fileREF.removeEventListener(Event.SELECT, onFileLoadHaneler);
					fileREF.addEventListener(Event.COMPLETE, onFileLoadHaneler);
					fileREF.load();
					break;
				case Event.COMPLETE:
					fileREF.removeEventListener(Event.COMPLETE, onFileLoadHaneler);
					setData(fileREF.data);
					break;
			}
		}
		
		public function setData(_data:ByteArray):void{
			isSWFSource = true;
			updateArmatures(_data ,extractXML(_data));
		}
		
		public function exportData():void{
			if(!xml || isExporting){
				return;
			}
			isExporting = true;
			if(!isSWFSource && dataImportID !=2 && (isTextureChanged || textureSortID == 1)){
				textureAtlasXML = packTexturesAndExportSWF(textureAtlasXML);
				xml[ConstValues.TEXTURE_ATLAS] = textureAtlasXML;
			}else{
				exportStart();
			}
		}
		
		private function exportStart():void{
			saveList = [];
			var _name:String = xml.attribute(ConstValues.A_NAME);
			var _data:ByteArray;
			
			switch(dataExportID){
				case 0:
					saveList.push({name:_name + ".png", data:getExportPNG()});
					exportStep();
					break;
				case 1:
					_data = getExportSWF();
					if(_data){
						saveList.push({name:_name + ".swf", data:_data});
					}else{
						Alert.show("当前导入的数据格式不支持导出 SWF！");
					}
					exportStep();
					break;
				case 2:
					Alert.show("暂不支持！");
					break;
				case 3:
					export.output();
					isExporting = false;
					break;
			}
		}
		
		private function getExportPNG():ByteArray{
			if(skeletonData.textureData.clip){
				skeletonData.textureData.updateBitmap();
				return getExportByteArray(PNGEncoder.encode(skeletonData.textureData.bitmap.bitmapData), xml.toXMLString());
			}else{
				return getExportByteArray(skeletonData.textureData.byteArray, xml.toXMLString());
			}
		}
		
		private function getExportSWF():ByteArray{
			if(skeletonData.textureData.clip){
				return getExportByteArray(skeletonData.textureData.byteArray, xml.toXMLString());
			}
			return null;
		}
		
		private function getExportByteArray(_byteArray:ByteArray, _string:String):ByteArray {
			var _xmlByte:ByteArray = new ByteArray();
			_xmlByte.writeUTFBytes(_string);
			_xmlByte.compress();
			
			var _byteArrayCopy:ByteArray = new ByteArray();
			_byteArrayCopy.writeBytes(_byteArray);
			
			_byteArrayCopy.position = _byteArrayCopy.length;
			_byteArrayCopy.writeBytes(_xmlByte);
			_byteArrayCopy.writeInt(_xmlByte.length);
			return _byteArrayCopy;
		}
		
		private function exportStep():void{
			if(saveList.length == 0){
				fileREF.removeEventListener(Event.CANCEL, onFileSaveHandler);
				fileREF.removeEventListener(Event.COMPLETE, onFileSaveHandler);
				isExporting = false;
				return;
			}
			var _data:Object = saveList.pop();
			fileREF.addEventListener(Event.CANCEL, onFileSaveHandler);
			fileREF.addEventListener(Event.COMPLETE, onFileSaveHandler);
			fileREF.save(_data.data, _data.name);
		}
		
		private function onFileSaveHandler(_e:Event):void{
			switch(_e.type){
				case Event.CANCEL:
				case Event.COMPLETE:
					exportStep();
					break;
			}
		}
		
		private var flaXML:XML;
		private var tempArr:Array;
		private var tempTextureAtlasXML:XML;
		private var tempSubTextureXMLList:XMLList;
		private var tempIndex:int;
		
		private function generateFLAXML(_arr:Array):void{
			tempArr = _arr;
			if(!tempArr || tempArr.length == 0){
				return;
			}
			alert = Alert.show("Waitting...");
			flaXML = null;
			container.addEventListener(Event.ENTER_FRAME, onGenerateFLAXMLHandler);
		}
		
		private function onGenerateFLAXMLHandler(_e:Event):void{
			if(tempArr.length == 0){
				container.removeEventListener(Event.ENTER_FRAME, onGenerateFLAXMLHandler);
				generateTextureSWF();
				return;
			}
			var _name:String = tempArr.pop();
			var _xml:XML = JSFL.generateArmature(_name);
			if(flaXML){
				mixSkeletonXML(flaXML, _xml);
			}else{
				flaXML = _xml;
			}
		}
		
		private function generateTextureSWF():void{
			tempTextureAtlasXML = flaXML.elements(ConstValues.TEXTURE_ATLAS)[0];
			tempSubTextureXMLList = tempTextureAtlasXML.elements(ConstValues.SUB_TEXTURE);
			tempIndex = tempSubTextureXMLList.length();
			JSFL.clearTextureSWFItem(tempIndex);
			container.addEventListener(Event.ENTER_FRAME, onGenerateTextureSWFHandler);
		}
		
		private function onGenerateTextureSWFHandler(_e:Event):void{
			if(tempIndex <= 0){
				container.removeEventListener(Event.ENTER_FRAME, onGenerateTextureSWFHandler);
				flaXML[ConstValues.TEXTURE_ATLAS] = packTexturesAndExportSWF(tempTextureAtlasXML);
				if(alert){
					PopUpManager.removePopUp(alert);
					alert = null;
				}
			}else{
				var _textureXML:XML = tempSubTextureXMLList[tempIndex - 1];
				JSFL.addTextureToSWFItem(_textureXML.attribute(ConstValues.A_NAME), _textureXML.name, tempIndex);
			}
			tempIndex --;
		}
		
		private function packTexturesAndExportSWF(_textureAtlasXML:XML):XML{
			_textureAtlasXML = JSFL.packTextures(uint(textureMaxWidthAC.getItemAt(textureMaxWidthID)), textureInterval, _textureAtlasXML);
			var _swfURL:String = JSFL.exportSWF();
			JSFL.trace(_swfURL);
			urlLoader.addEventListener(Event.COMPLETE, onURLLoaderCompleteHandler);
			urlLoader.dataFormat = URLLoaderDataFormat.BINARY;
			urlLoader.load(new URLRequest(_swfURL));
			return _textureAtlasXML;
		}
		
		private function onURLLoaderCompleteHandler(_e:Event):void{
			isTextureChanged = false;
			isSWFSource = false;
			urlLoader.removeEventListener(Event.COMPLETE, onURLLoaderCompleteHandler);
			updateArmatures(_e.target.data, flaXML);
			flaXML = null;
		}
		
		private function updateArmatures(_byteArray:ByteArray, _xml:XML = null):void{
			isDraged = false;
			xml = _xml || xml;
			armaturesXML = xml.elements(ConstValues.ARMATURES)[0];
			animationsXML = xml.elements(ConstValues.ANIMATIONS)[0];
			textureAtlasXML = xml.elements(ConstValues.TEXTURE_ATLAS)[0];
			
			armaturesMC.source = armaturesXML.elements(ConstValues.ARMATURE);
			texturesMC.source = textureAtlasXML.elements(ConstValues.SUB_TEXTURE);
			
			armatures = {};
			if(armature){
				armature.animation.stop();
			}
			if(skeletonData){
				skeletonData.dispose();
			}
			skeletonData = generateSkeletonData(_byteArray, xml.copy());
			
			skeletonData.textureData.updateBitmap();
			
			setTimeoutID = setTimeout(sourceUpdate, 400);
		}
		
		private function addExportSingleTexture():void{
			export = new SimpleExport(null);
			if(skeletonData.textureData.clip){
				export.textures.add( TextureExtractor.extract(skeletonData.textureData.clip ) );
				export.skeletonXML = xml.copy();
			}
			
			export.export();
		}
		
		
		private function sourceUpdate():void{
			clearTimeout(setTimeoutID);
			addExportSingleTexture();
			textureDisplay = skeletonData.textureData.bitmap || skeletonData.textureData.clip;
			BaseFactory.lastInstance.skeletonData = skeletonData;
			
			dispatchEvent(new Event(SOURCE_UPDATE));
			if(isExporting){
				exportStart();
			}
		}
		
		private function changeMovement():void{
			var _armatureName:String = armature.info.name;
			var _movementName:String = movementXML.attribute(ConstValues.A_NAME);
			var _animationData:AnimationData = skeletonData.getAnimationData(_armatureName);
			_animationData.addData(generateMovementData(movementXML));
			armature.animation.play(_movementName);
			if(!this.isSWFSource){
				JSFL.changeMovement(_armatureName, _movementName, movementXML);
			}
		}
		
		private function changeMovementBone():void{
			var _armatureName:String = armature.info.name;
			var _movementName:String = movementXML.attribute(ConstValues.A_NAME);
			var _boneName:String = movementBoneXML.attribute(ConstValues.A_NAME);
			
			var _animationData:AnimationData = skeletonData.getAnimationData(_armatureName);
			var _movementData:MovementData = _animationData.getData(_movementName);
			var _movementBoneData:MovementBoneData = _movementData.getData(_boneName);
			
			_movementBoneData.scale = Number(movementBoneXML.attribute(ConstValues.A_MOVEMENT_SCALE));
			_movementBoneData.delay = Number(movementBoneXML.attribute(ConstValues.A_MOVEMENT_DELAY));
			if(_movementBoneData.delay > 0){
				_movementBoneData.delay -= 1;
			}
			
			armature.animation.play(_movementName);
			if(!this.isSWFSource){
				var _movementXMLCopy:XML = movementXML.copy();
				delete _movementXMLCopy.elements(ConstValues.BONE).*;
				JSFL.changeMovement(_armatureName, _movementName, _movementXMLCopy);
			}
		}
		
		public function updateArmature(_armatureXML:XML):void{
			armatureXML = _armatureXML;
			var _armatureName:String = armatureXML.attribute(ConstValues.A_NAME);
			animationXML = animationsXML.elements(ConstValues.ANIMATION).(attribute(ConstValues.A_NAME) == _armatureName)[0];
			if(armatureXML){
				movementsMC.source = getMovementList();
				bonesMC.removeAll();
				bonesMC.source = getBoneList();
			}else{
				movementsMC.source = null
				bonesMC.removeAll();
				bonesMC.source = null;
			}
			
			if(armature && armature.display && container.contains(armature.display as DisplayObject)){
				container.removeChild(armature.display as DisplayObject);
			}
			armature = armatures[_armatureName];
			if(!armature){
				armatures[_armatureName] = armature = BaseFactory.lastInstance.buildArmature(_armatureName);
			}
			armature.armatureEventCallback = armatureEventCallback;
			if(armature.display){
				container.addChild(armature.display as DisplayObject);
			}
			
			dispatchEvent(new Event(ARMATURE_UPDATE));
		}
		
		private function armatureEventCallback(_eventType:String, _movementID:String):void{
			//trace(_eventType, _movementID);
		}
		
		public function updateMovement(_movementXML:XML):void{
			movementXML = _movementXML;
			if(movementXML && armature){
				armature.animation.play(String(movementXML.attribute(ConstValues.A_NAME)));
			}
			maxWidth = 0;
			maxHeight = 0;
			dispatchEvent(new Event(MOVEMENT_UPDATE));	
		}
		
		public function updateMovementBone(_movementBoneXML:XML):void{
			if(_movementBoneXML && movementXML){
				var _boneName:String = _movementBoneXML.attribute(ConstValues.A_NAME);
				movementBoneXML = movementXML.elements(ConstValues.BONE).(attribute(ConstValues.A_NAME) == _boneName)[0];
			}else{
				movementBoneXML = null;
			}
			dispatchEvent(new Event(BONE_UPDATE));
		}
		
		public function updateTexture():void{
			if(isSWFSource){
				return;
			}
			switch(textureSortID){
				case 0:
					JSFL.packTextures(uint(textureMaxWidthAC.getItemAt(textureMaxWidthID)), textureInterval);
					isTextureChanged = true;
					break;
			}
		}
		
		public function updateBoneParent(_boneCopy:XML):void{
			if(isSWFSource){
				return;
			}
			var _parent:XML = _boneCopy.parent();
			var _name:String = _boneCopy.attribute(ConstValues.A_NAME);
			var _parentName:String = _parent.attribute(ConstValues.A_NAME);
			
			var _boneXML:XML = armatureXML.elements(ConstValues.BONE).(attribute(ConstValues.A_NAME) == _name)[0];
			var _isChange:Boolean;
			if(_parent.name() == _boneCopy.name()){
				if(_boneXML.attribute(ConstValues.A_PARENT) != _parentName){
					_boneXML[ConstValues.AT + ConstValues.A_PARENT] = _parentName;
					_isChange = true;
				}
			}else{
				if(_boneXML.attribute(ConstValues.A_PARENT).length()>0){
					_isChange = true;
					delete _boneXML[ConstValues.AT + ConstValues.A_PARENT];
				}
			}
			if(_isChange){
				JSFL.changeArmatureConnection(armatureXML.attribute(ConstValues.A_NAME), armatureXML);
			}
		}
		
		private function getMovementList():XMLList{
			var _animationXML:XML = animationsXML[ConstValues.ANIMATION].(attribute(ConstValues.A_NAME) == armatureXML.attribute(ConstValues.A_NAME))[0];
			if(_animationXML){
				return _animationXML[ConstValues.MOVEMENT];
			}
			return null;
		}
		
		private function getBoneList():XMLList{
			var _xmlList:XMLList = armatureXML.copy()[ConstValues.BONE];
			var _dic:Object = {};
			var _parentXML:XML;
			var _parentName:String;
			var _boneXML:XML;
			var _length:int = _xmlList.length();
			for(var _i:int = _length-1;_i >= 0;_i --){
				_boneXML = _xmlList[_i];
				_dic[_boneXML.attribute(ConstValues.A_NAME)] = _boneXML;
				_parentName = _boneXML.attribute(ConstValues.A_PARENT);
				if (_parentName){
					_parentXML = _dic[_parentName] || _xmlList.(attribute(ConstValues.A_NAME) == _parentName)[0];
					if (_parentXML){
						delete _xmlList[_i];
						_parentXML.appendChild(_boneXML);
					}
				}
			}
			return _xmlList;
		}
		
		private static function mixSkeletonXML(_xml1:XML, _xml2:XML):void{
			var _xmlList1:XMLList;
			var _xmlList2:XMLList;
			var _node1:XML;
			var _node2:XML;
			var _name:String;
			
			_xmlList1 = _xml1.elements(ConstValues.ARMATURES).elements(ConstValues.ARMATURE);
			_xmlList2 = _xml2.elements(ConstValues.ARMATURES).elements(ConstValues.ARMATURE);
			for each(_node2 in _xmlList2){
				_name = _node2.attribute(ConstValues.A_NAME);
				_node1 = _xmlList1.(attribute(ConstValues.A_NAME) == _name)[0];
				if(_node1){
					delete _xmlList1[_node1.childIndex()];
				}
				_xml1.elements(ConstValues.ARMATURES).appendChild(_node2);
			}
			
			_xmlList1 = _xml1.elements(ConstValues.ANIMATIONS).elements(ConstValues.ANIMATION);
			_xmlList2 = _xml2.elements(ConstValues.ANIMATIONS).elements(ConstValues.ANIMATION);
			for each(_node2 in _xmlList2){
				_name = _node2.attribute(ConstValues.A_NAME);
				_node1 = _xmlList1.(attribute(ConstValues.A_NAME) == _name)[0];
				if(_node1){
					delete _xmlList1[_node1.childIndex()];
				}
				_xml1.elements(ConstValues.ANIMATIONS).appendChild(_node2);
			}
			
			_xmlList1 = _xml1.elements(ConstValues.TEXTURE_ATLAS).elements(ConstValues.SUB_TEXTURE);
			_xmlList2 = _xml2.elements(ConstValues.TEXTURE_ATLAS).elements(ConstValues.SUB_TEXTURE);
			for each(_node2 in _xmlList2){
				_name = _node2.attribute(ConstValues.A_NAME);
				_node1 = _xmlList1.(attribute(ConstValues.A_NAME) == _name)[0];
				if(_node1){
					delete _xmlList1[_node1.childIndex()];
				}
				_xml1.elements(ConstValues.TEXTURE_ATLAS).appendChild(_node2);
			}
		}
	}
}