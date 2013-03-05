﻿var Skeleton = {};
(function(){

var SKELETON = "skeleton";

var ARMATURES = "armatures";
var ARMATURE = "armature";
var BONE = "b";
var DISPLAY = "d";

var ANIMATIONS = "animations";
var ANIMATION = "animation";
var MOVEMENT = "mov";
var FRAME = "f";

var TEXTURE_ATLAS = "TextureAtlas";
var SUB_TEXTURE = "SubTexture";

var AT = "@";
var A_FRAME_RATE = "frameRate";
var A_NAME = "name";
var A_START = "st";
var A_DURATION = "dr";
var A_DURATION_TO = "to";
var A_DURATION_TWEEN = "drTW";
var A_LOOP = "lp";
var A_MOVEMENT_SCALE = "sc";
var A_MOVEMENT_DELAY = "dl";

var A_PARENT = "parent";
var A_X = "x";
var A_Y = "y";
var A_SCALE_X = "cX";
var A_SCALE_Y = "cY";
var A_SKEW_X = "kX";
var A_SKEW_Y = "kY";
var A_Z = "z";
var A_DISPLAY_INDEX = "dI";
var A_EVENT = "evt";
var A_SOUND = "sd";
var A_SOUND_EFFECT = "sdE";
var A_TWEEN_EASING ="twE";
var A_TWEEN_ROTATE_ ="twR_";
var A_TWEEN_ROTATE ="twR";
var A_IS_ARMATURE = "isArmature";
var A_MOVEMENT = "mov";
var A_ALPHA = "alpha";
var A_RED = "red";
var A_GREEN = "green";
var A_BLUE = "blue";

var A_WIDTH = "width";
var A_HEIGHT = "height";
var A_PIVOT_X = "pX";
var A_PIVOT_Y = "pY";

var V_SOUND_LEFT = "l";
var V_SOUND_RIGHT = "r";
var V_SOUND_LEFT_TO_RIGHT = "lr";
var V_SOUND_RIGHT_TO_LEFT = "rl";
var V_SOUND_FADE_IN = "in";
var V_SOUND_FADE_OUT = "out";

var MOVIE_CLIP = "movie clip";
var GRAPHIC = "graphic";
var STRING = "string";
var LABEL_TYPE_NAME = "name";
var EVENT_PREFIX = "@";
var MOVEMENT_PREFIX = "#";
var NO_EASING = "^";
var DELIM_CHAR = "|";
var UNDERLINE_CHAR = "_";

var SKELETON_PANEL = "SkeletonAnimationDesignPanel";
var ARMATURE_DATA = "armatureData";
var ANIMATION_DATA = "animationData";

var TEXTURE_SWF_ITEM = "textureSWFItem";
var TEXTURE_SWF = "armatureTextureSWF.swf";

var helpPoint = {x:0, y:0};

var currentDom;
var currentLibrary;
var currentDomName;
var currentItemBackup;
var currentFrameBackup;

var xml;
var armaturesXML;
var animationsXML;
var textureAtlasXML;

var armatureXML;
var animationXML;
var armatureConnectionXML;

function trace(){
	var _str = "";
	for(var _i = 0;_i < arguments.length;_i ++){
		if(_i!=0){
			_str += ", ";
		}
		_str += arguments[_i];
	}
	fl.trace(_str);
}

function formatNumber(_num, _retain){
	_retain = _retain || 100;
	return Math.round(_num * _retain) / 100;
}

function replaceString(_strOld, _str, _rep){
	if(_strOld){
		return _strOld.split(_str).join(_rep);
	}
	return "";
}

//to determine whether the layer is blank layer
function isBlankLayer(_layer){
	for each(var _frame in filterKeyFrames(_layer.frames)){
		if(_frame.elements.length){
			return false;
		}
	}
	return true;
}

//filter key frames from a frame array
function filterKeyFrames(_frames){
	var _framesCopy = [];
	for each(var _frame in _frames){
		if(_framesCopy.indexOf(_frame) >= 0){
			continue;
		}
		_framesCopy.push(_frame);
	}
	return _framesCopy;
}

function errorDOM(){
	if(!currentDom){
		alert("cannot open FLA file！");
		return true;
	}
	return false;
}

//change object name if its name is invalid.
function formatName(_obj){
	var _name = _obj.name;
	if(!_name){
		_obj.name = _name = "unnamed" + Math.round(Math.random()*10000);
	}else if(_name.indexOf(DELIM_CHAR) >= 0){
		_obj.name = _name = replaceString(_name, DELIM_CHAR, "");
	}
	return _name;
}

//handle conflict object name
function formatSameName(_obj, _dic){
	var _i = 0;
	var _name = formatName(_obj);
	while(_dic[_name]){
		_name = _obj.name + _i;
		_i ++;
	}
	if(_i > 0){
		_obj.name = _name;
	}
	_dic[_name] = true;
	return _name;
}

//to determine whether the frame is not a easing frame.
function isNoEasingFrame(_frame){
	return _frame.labelType == LABEL_TYPE_NAME && _frame.name.indexOf(NO_EASING) >= 0;
}

//To determine whether the frame is special frame
function isSpecialFrame(_frame, _framePrefix, _returnName){
	var _b = _frame.labelType == LABEL_TYPE_NAME && _frame.name.indexOf(_framePrefix) >= 0 && _frame.name.length > 1;
	if(_b && _returnName){
		var _arr = _frame.name.split(DELIM_CHAR);
		for each(var _str in _arr){
			if(_str.indexOf(_framePrefix) == 0){
				return _str.substr(1);
			}
		}
		//_frame.name is incorrect special frame name
		return false;
	}
	return _b;
}

//To determine whether the frame is a main frame
function isMainFrame(_frame){
	return _frame.labelType == LABEL_TYPE_NAME && !isNoEasingFrame(_frame) && !isSpecialFrame(_frame, EVENT_PREFIX) && !isSpecialFrame(_frame, MOVEMENT_PREFIX);
}

//To determine whether the layer is main label layer
function isMainLayer(_layer){
	for each(var _frame in filterKeyFrames(_layer.frames)){
		if(isMainFrame(_frame)){
			return true;
		}
	}
	return false;
}

//To determine whether the item is valide armature.
//If yes, return mainLayer and boneLayers
function isArmatureItem(_item, _isChildArmature){
	var _layersFiltered = [];
	var _mainLayer;
	for each(var _layer in _item.timeline.layers){
		switch(_layer.layerType){
			case "folder":
			case "guide":
			case "mask":
				break;
			default:
				if(isMainLayer(_layer)){
					_mainLayer = _layer;
				}else if(!isBlankLayer(_layer)){
					_layersFiltered.unshift(_layer);
				}
				break;
		}
	}
	
	if(_layersFiltered.length > 0){
		if(_mainLayer){
			_layersFiltered.unshift(_mainLayer);
			return _layersFiltered;
		}else if(_isChildArmature && _item.timeline.frameCount > 1){
			_mainLayer = {};
			_mainLayer.frames = [];
			_mainLayer.frameCount = _item.timeline.frameCount;
			
			var _frame = { };
			_frame.labelType = LABEL_TYPE_NAME;
			_frame.name = "unnamed";
			_frame.startFrame = 0;
			_frame.duration = _mainLayer.frameCount;
			
			_mainLayer.frames.push(_frame);
			_layersFiltered.unshift(_mainLayer);
			return _layersFiltered;
		}
	}
	return null;
}

//filter bone symbol from all elements in a frame.
function getBoneSymbol(_elements){
	for each(var _element in _elements){
		if(_element.symbolType == MOVIE_CLIP || _element.symbolType == GRAPHIC){
			return _element;
		}
	}
	return null;
}

//get bone by name from a frame 
function getBoneFromLayers(layers, _boneName, _frameIndex){
	for each(var _layer in layers){
		if(_layer.name == _boneName){
			return getBoneSymbol(_layer.frames[_frameIndex].elements);
		}
	}
	return null;
}

//write armature connection data
function setArmatureConnection(_item, _data){
	_item.addData(ARMATURE_DATA, STRING, _data);
	//Jsfl api Or Flash pro bug
	_item.symbolType = GRAPHIC;
	_item.symbolType = MOVIE_CLIP;
}

function getMovementXML(_movementName, _duration, _item){
	var _xml = <{MOVEMENT} {A_NAME} = {_movementName}/>;
	if(_item.hasData(ANIMATION_DATA)){
		var _animationXML = XML(_item.getData(ANIMATION_DATA));
		var _movementXML = _animationXML[MOVEMENT].(@name == _movementName)[0];
	}
	_xml[AT + A_DURATION] = _duration;
	if(_movementXML){
		_xml[AT + A_DURATION_TO] = _movementXML[AT + A_DURATION_TO];
	}else{
		_xml[AT + A_DURATION_TO] = 6;
	}
	if(_duration > 1){
		if(_movementXML){
			if(_xml[AT + A_DURATION] == _movementXML[AT + A_DURATION]){
				_xml[AT + A_DURATION_TWEEN] = _movementXML[AT + A_DURATION_TWEEN];
			}else{
				_xml[AT + A_DURATION_TWEEN] = _duration;
			}
			_xml[AT + A_LOOP] = _movementXML[AT + A_LOOP];
			_xml[AT + A_TWEEN_EASING] = _movementXML[AT + A_TWEEN_EASING].length()?_movementXML[AT + A_TWEEN_EASING]:NaN;
		}else{
			_xml[AT + A_DURATION_TWEEN] = _duration > 2?_duration:10;
			if(_duration == 2){
				_xml[AT + A_LOOP] = 1;
				_xml[AT + A_TWEEN_EASING] = 2;
			}else{
				_xml[AT + A_LOOP] = 0;
				_xml[AT + A_TWEEN_EASING] = NaN;
			}
		}
	}
	return _xml;
}

function getMovementBoneXML(_movementXML, _boneName, _item){
	var _xml = _movementXML[BONE].(@name == _boneName)[0];
	if(!_xml){
		_xml = <{BONE} {A_NAME} = {_boneName}/>;
		_xml[AT + A_MOVEMENT_SCALE] = 1;
		_xml[AT + A_MOVEMENT_DELAY] = 0;
		if(_item.hasData(ANIMATION_DATA)){
			var _animationXML = XML(_item.getData(ANIMATION_DATA));
			_movementName = _movementXML[AT + A_NAME];
			var _movementXMLBackup = _animationXML[MOVEMENT].(@name == _movementName)[0];
			if(_movementXMLBackup){
				var _boneXML = _movementXMLBackup[BONE].(@name == _boneName)[0];
				if(_boneXML){
					_xml[AT + A_MOVEMENT_SCALE] = _boneXML[AT + A_MOVEMENT_SCALE];
					_xml[AT + A_MOVEMENT_DELAY] = _boneXML[AT + A_MOVEMENT_DELAY];
				}
			}
		}
		_movementXML.appendChild(_xml);
	}
	return _xml;
}

function getBoneXML(_name, _frameXML){
	var _xml = armatureXML[BONE].(@name == _name)[0];
	if(!_xml){
		_xml = <{BONE} {A_NAME} = {_name}/>;
		var _connectionXML = armatureConnectionXML[BONE].(@name == _name)[0];
		if(_connectionXML && _connectionXML[AT + A_PARENT][0]){
			_xml[AT + A_PARENT] = _connectionXML[AT + A_PARENT];
		}
		_xml[AT + A_X] = _frameXML[AT + A_X];
		_xml[AT + A_Y] = _frameXML[AT + A_Y];
		_xml[AT + A_SKEW_X] = _frameXML[AT + A_SKEW_X];
		_xml[AT + A_SKEW_Y] = _frameXML[AT + A_SKEW_Y];
		_xml[AT + A_SCALE_X] = _frameXML[AT + A_SCALE_X];
		_xml[AT + A_SCALE_Y] = _frameXML[AT + A_SCALE_Y];
		_xml[AT + A_Z] = _frameXML[AT + A_Z];
		_xml[AT + A_ALPHA] = _frameXML[AT + A_ALPHA];
		_xml[AT + A_RED] = _frameXML[AT + A_RED];
		_xml[AT + A_GREEN] = _frameXML[AT + A_GREEN];
		_xml[AT + A_BLUE] = _frameXML[AT + A_BLUE];
		armatureXML.appendChild(_xml);
	}
	return _xml;
}

function getDisplayXML(_boneXML, _imageName, _isArmature){
	var _xml = _boneXML[DISPLAY].(@name == _imageName)[0];
	if(!_xml){
		_xml = <{DISPLAY} {A_NAME} = {_imageName}/>;
		if(_isArmature){
			_xml[AT + A_IS_ARMATURE] = 1;
		}
		_boneXML.appendChild(_xml);
	}
	return _xml;
}

function getTextureXML(_imageName){
	var _xml = textureAtlasXML[SUB_TEXTURE].(@name == _imageName)[0];
	if(!_xml){
		_xml = <{SUB_TEXTURE} {A_NAME} = {_imageName}/>;
		textureAtlasXML.appendChild(_xml);
	}
	return _xml;
}

function generateMovement(_item, _mainFrame, _layers){
	var _start = _mainFrame.frame.startFrame;
	var _duration = _mainFrame.duration;
	var _movementXML = getMovementXML(_mainFrame.frame.name, _duration, _item);
	
	var _boneNameDic = {};
	var _boneZDic = {};
	var _zList = [];
	var _frameStart;
	var _frameDuration;
	var _boneList;
	var _z;
	var _i;
	var _movementBoneXML;
	var _frameXML;
	var _symbol;
	var _boneName;
	
	for each(var _layer in _layers){
		_boneName = formatName(_layer);
		_boneZDic[_boneName] = _boneZDic[_boneName] || [];
		_movementBoneXML = null;
		for each(var _frame in filterKeyFrames(_layer.frames.slice(_start, _start + _duration))){
			if(_frame.startFrame < _start){
				_frameStart = 0;
				_frameDuration = _frame.duration - _start + _frame.startFrame;
			}else if(_frame.startFrame + _frame.duration > _start + _duration){
				_frameStart = _frame.startFrame - _start;
				_frameDuration = _duration - _frame.startFrame + _start;
			}else{
				_frameStart = _frame.startFrame - _start;
				_frameDuration= _frame.duration;
			}
			_symbol = getBoneSymbol(_frame.elements);
			if(!_symbol){
				continue;
			}
			if(!_movementBoneXML){
				_movementBoneXML = getMovementBoneXML(_movementXML, _boneName, _item);
			}
			for(_i = _frameStart ;_i < _frameStart + _frameDuration;_i ++){
				_z = _zList[_i];
				if(isNaN(_z)){
					_zList[_i] = _z = 0;
				}else{
					_zList[_i] = ++_z;
				}
			}
			_boneList = _boneZDic[_boneName];
			for(_i = _frameStart;_i < _frameStart + _frameDuration;_i ++){
				if(!isNaN(_boneList[_i])){
					_boneNameDic[_boneName] = true;
					_boneName = formatSameName(_layer, _boneNameDic);
					_boneList = _boneZDic[_boneName] = [];
					_movementBoneXML = getMovementBoneXML(_movementXML, _boneName, _item);
				}
				_boneList[_i] = _z;
			}
			
			if(_frame.tweenType == "motion object"){
				
				break;
			}
			_frameXML = generateFrame(_frame, _boneName, _symbol, _z, _layers, Math.max(_frame.startFrame, _start));
			addFrameToMovementBone(_frameXML, _frameStart, _frameDuration, _movementBoneXML);
		}
	}
	
	var _prevStart;
	var _prevDuration;
	var _frameIndex;
	
	for each(var _movementBoneXML in _movementXML[BONE]){
		_boneName = _movementBoneXML[AT + A_NAME];
		var _prevFrameXML = null;
		for each(_frameXML in _movementBoneXML[FRAME]){
			_frameStart = Number(_frameXML[AT + A_START]);
			if(_frameXML.childIndex() == 0){
				if(_frameStart > 0){
					_movementBoneXML.prependChild(<{FRAME} {A_DURATION} = {_frameStart} {A_DISPLAY_INDEX} = "-1"/>);
				}
			}else {
				_prevStart = Number(_prevFrameXML[AT + A_START]);
				_prevDuration = Number(_prevFrameXML[AT + A_DURATION]);
				if(_frameStart > _prevStart + _prevDuration){
					_movementBoneXML.insertChildBefore(_frameXML, <{FRAME} {A_DURATION} = {_frameStart - _prevStart - _prevDuration} {A_DISPLAY_INDEX} = "-1"/>);
				}
			}
			if(_frameXML.childIndex() == _movementBoneXML[FRAME].length() - 1){
				_frameStart = Number(_frameXML[AT + A_START]);
				_prevDuration = Number(_frameXML[AT + A_DURATION]);
				if(_frameStart + _prevDuration < _duration){
					_movementBoneXML.appendChild(<{FRAME} {A_DURATION} = {_duration - _frameStart - _prevDuration} {A_DISPLAY_INDEX} = "-1"/>);
				}
			}
			//tweenRotate property is for the end point of tween instead of start point
			//sometimes, x0 need to be ingored
			if(_prevFrameXML && _prevFrameXML[AT + A_TWEEN_ROTATE_][0]){
				var _dSkY = Number(_frameXML[AT + A_SKEW_Y]) - Number(_prevFrameXML[AT + A_SKEW_Y]);
				if(_dSkY < -180){
					_dSkY += 360;
				}
				if(_dSkY > 180){
					_dSkY -= 360;
				}
				_tweenRotate = Number(_prevFrameXML[AT + A_TWEEN_ROTATE_]);
				if(_dSkY !=0){
					if(_dSkY < 0){
						if(_tweenRotate >= 0){
							_tweenRotate ++;
						}
					}else{
						if(_tweenRotate < 0){
							_tweenRotate --;
						}
					}
				}
				_frameXML[AT + A_TWEEN_ROTATE] = _tweenRotate;
				delete _prevFrameXML[AT + A_TWEEN_ROTATE_];
			}
			
			_prevFrameXML = _frameXML;
		}
	}
	delete _movementXML[BONE][FRAME][AT + A_START];
	
	generateMovementEventFrames(_movementXML, _mainFrame);
	
	animationXML.appendChild(_movementXML);
}

function generateFrame(_frame, _boneName, _symbol, _z){
	var _frameXML = <{FRAME}/>;
	_frameXML[AT + A_X] = formatNumber(_symbol.x);
	_frameXML[AT + A_Y] = formatNumber(_symbol.y);
	var _rotate = _symbol.skewX;
	while(_rotate > 180){
		_rotate -= 360;
	}
	while(_rotate < -180){
		_rotate += 360;
	}
	_frameXML[AT + A_SKEW_X] = formatNumber(_symbol.skewX);
	_frameXML[AT + A_SKEW_Y] = formatNumber(_symbol.skewY);
	_frameXML[AT + A_SCALE_X] = formatNumber(_symbol.scaleX);
	_frameXML[AT + A_SCALE_Y] = formatNumber(_symbol.scaleY);
	_frameXML[AT + A_Z] = _z;

	_frameXML[AT + A_ALPHA] = _symbol.colorAlphaPercent;
	_frameXML[AT + A_RED] = _symbol.colorRedPercent;
	_frameXML[AT + A_GREEN] = _symbol.colorGreenPercent;
	_frameXML[AT + A_BLUE] = _symbol.colorBluePercent;
	
	var _boneXML = getBoneXML(_boneName, _frameXML);
	
	var _imageItem = _symbol.libraryItem;
	var _imageName = formatName(_imageItem);
	var _isChildArmature = _symbol.symbolType == MOVIE_CLIP;
	var _isArmature = isArmatureItem(_imageItem, _isChildArmature);
	var _displayXML = getDisplayXML(_boneXML, _imageName, _isArmature);
	_frameXML[AT + A_DISPLAY_INDEX] = _displayXML.childIndex();
	if(_isArmature){
		var _backupArmatureXML = armatureXML;
		var _backupAnimationXML = animationXML;
		var _backupArmatureConnectionXML = armatureConnectionXML;
		
		Skeleton.generateArmature(_imageName, null, _isChildArmature);
		
		armatureXML = _backupArmatureXML;
		animationXML = _backupAnimationXML;
		armatureConnectionXML = _backupArmatureConnectionXML;
		
	}else{
		getTextureXML(_imageName);
	}
	
	var _str = isSpecialFrame(_frame, MOVEMENT_PREFIX, true);
	if(_str){
		_frameXML[AT + A_MOVEMENT] = _str;
	}
	
	//generate tween easing frame
	if(isNoEasingFrame(_frame)){
		//For key frame with '^', will not generate tween easing.
		_frameXML[AT + A_TWEEN_EASING] = NaN;
	}else if(_frame.tweenType == "motion"){
		_frameXML[AT + A_TWEEN_EASING] = formatNumber(_frame.tweenEasing * 0.01);
		var _tweenRotate = NaN;
		switch(_frame.motionTweenRotate){
			case "clockwise":
				_tweenRotate = _frame.motionTweenRotateTimes;
				break;
			case "counter-clockwise":
				_tweenRotate = - _frame.motionTweenRotateTimes;
				break;
		}
		if(!isNaN(_tweenRotate)){
			_frameXML[AT + A_TWEEN_ROTATE_] = _tweenRotate;
		}
	}
	
	//event
	_str = isSpecialFrame(_frame, EVENT_PREFIX, true);
	if(_str){
		_frameXML[AT + A_EVENT] = _str;
	}

	//sound
	if(_frame.soundName){
		_frameXML[AT + A_SOUND] = _frame.soundLibraryItem.linkageClassName || _frame.soundName;
		var _soundEffect;
		switch(_frame.soundEffect){
			case "left channel":
				_soundEffect = V_SOUND_LEFT;
				break;
			case "right channel":
				_soundEffect = V_SOUND_RIGHT;
				break;
			case "fade left to right":
				_soundEffect = V_SOUND_LEFT_TO_RIGHT;
				break;
			case "fade right to left":
				_soundEffect = V_SOUND_RIGHT_TO_LEFT;
				break;
			case "fade in":
				_soundEffect = V_SOUND_FADE_IN;
				break;
			case "fade out":
				_soundEffect = V_SOUND_FADE_OUT;
				break;
		}
		if(_soundEffect){
			_frameXML[AT + A_SOUND_EFFECT] = _soundEffect;
		}
	}
	
	return _frameXML;
}

function generateMovementEventFrames(_movementXML, _mainFrame){
	if(_mainFrame.frames.length > 1){
		var _start = _mainFrame.frame.startFrame;
		for each(var _frame in _mainFrame.frames){
			var _eventXML = <{FRAME} {A_START} = {_frame.startFrame - _start} {A_DURATION} = {_frame.duration}/>;
			var _event = isSpecialFrame(_frame, EVENT_PREFIX, true);
			var _movement = isSpecialFrame(_frame, MOVEMENT_PREFIX, true);
			var _sound = _frame.soundName && (_frame.soundLibraryItem.linkageClassName || _frame.soundName);
			if(_event){
				_eventXML[AT + A_EVENT] = _event;
			}
			if(_movement){
				_eventXML[AT + A_MOVEMENT] = _movement;
			}
			if(_sound){
				_frameXML[AT + A_SOUND] = _sound;
			}
			_movementXML.appendChild(_eventXML);
		}
	}
}

function addFrameToMovementBone(_frameXML, _start, _duration, _movementBoneXML){
	_frameXML[AT + A_START] = _start;
	_frameXML[AT + A_DURATION] = _duration;
	for each(var _eachFrameXML in _movementBoneXML[FRAME]){
		if(Number(_eachFrameXML[AT + A_START]) > _start){
			_movementBoneXML.insertChildBefore(_eachFrameXML, _frameXML);
			return;
		}
	}
	_movementBoneXML.appendChild(_frameXML);
}

Skeleton.getArmatureList = function(_isSelected){
	fl.outputPanel.clear();
	currentDom = fl.getDocumentDOM();
	currentLibrary = currentDom?currentDom.library:null;
	if(errorDOM()){
		return false;
	}
	var _timeline = currentDom.getTimeline();
	
	currentItemBackup = _timeline.libraryItem;
	if(currentItemBackup){
		currentFrameBackup = _timeline.currentFrame;
	}
	currentDom.exitEditMode();
	
	currentDomName = currentDom.name.split(".")[0];
	
	var _items = _isSelected?currentLibrary.getSelectedItems():currentLibrary.items;
	
	//first arr item is current fla domname
	var _importItems = [currentDomName];
	for each(var _item in _items){
		if((_item.symbolType == MOVIE_CLIP || _item.symbolType == GRAPHIC) && isArmatureItem(_item)){
			formatName(_item);
			_importItems.push(_item.name);
		}
	}
	return _importItems;
}

Skeleton.generateArmature = function(_armatureName, _isNewXML, _isChildArmature){
	var _item = currentLibrary.items[currentLibrary.findItemIndex(_armatureName)];
	if(!_item){
		return false;
	}
	if(_isNewXML){
		xml = <{SKELETON} {A_NAME} = {currentDomName} {A_FRAME_RATE} = {currentDom.frameRate}/>;
		armaturesXML = <{ARMATURES}/>;
		animationsXML = <{ANIMATIONS}/>;
		textureAtlasXML = <{TEXTURE_ATLAS}/>;
		xml.appendChild(armaturesXML);
		xml.appendChild(animationsXML);
		xml.appendChild(textureAtlasXML);
	}
	if(armaturesXML[ARMATURE].(@name == _armatureName)[0]){
		return false;
	}
	
	var _layersFiltered = isArmatureItem(_item, _isChildArmature);
	var _mainLayer = _layersFiltered.shift();
	
	armatureXML = <{ARMATURE} {A_NAME} = {_armatureName}/>;
	armaturesXML.appendChild(armatureXML);
	animationXML = <{ANIMATION} {A_NAME} = {_armatureName}/>;
	//if frame count > 1, the skeleton have animation.
	if(_mainLayer.frameCount > 1){
		animationsXML.appendChild(animationXML);
	}
	
	armatureConnectionXML = _item.hasData(ARMATURE_DATA)?XML(_item.getData(ARMATURE_DATA)):armatureXML;
	
	var _keyFrames = filterKeyFrames(_mainLayer.frames);
	var _length = _keyFrames.length;
	var _nameDic = {};
	
	var _frame;
	var _mainFrame;
	var _isEndFrame;
	
	for(var _iF = 0;_iF < _length;_iF ++){
		_frame = _keyFrames[_iF];
		if(isMainFrame(_frame)){
			//new main frame
			_mainFrame = {};
			_mainFrame.frame = _frame;
			_mainFrame.duration = _frame.duration;
			_mainFrame.frames = [_frame];
			formatSameName(_frame, _nameDic);
		}else if(_mainFrame){
			//continue
			_mainFrame.duration += _frame.duration;
			_mainFrame.frames.push(_frame);
		}else{
			//ignore
			continue;
		}
		_isEndFrame = _iF + 1 == _length || isMainFrame(_keyFrames[_iF + 1]);
		if(_mainFrame && _isEndFrame){
			//end
			generateMovement(_item, _mainFrame, _layersFiltered);
		}
	}
	//setArmatureConnection(_item, armatureXML.toXMLString());
	return xml.toXMLString();
}

Skeleton.clearTextureSWFItem = function(){
	if(!currentLibrary.itemExists(TEXTURE_SWF_ITEM)){
		currentLibrary.addNewItem(MOVIE_CLIP, TEXTURE_SWF_ITEM);
	}
	currentLibrary.editItem(TEXTURE_SWF_ITEM);
	xml = null;
	armaturesXML = null;
	animationsXML = null;
	textureAtlasXML = null;

	armatureXML = null;
	animationXML = null;
	armatureConnectionXML = null;
	
	var _timeline = currentDom.getTimeline();
	_timeline.currentLayer = 0;
	_timeline.removeFrames(0, _timeline.frameCount);
	_timeline.insertBlankKeyframe(0);
	_timeline.insertBlankKeyframe(1);
}

Skeleton.addTextureToSWFItem = function(_textureName, _isLast){
	var _item = currentLibrary.items[currentLibrary.findItemIndex(_textureName)];
	if(!_item){
		return false;
	}
	
	var _timeline = currentDom.getTimeline();
	
	_timeline.currentFrame = 0;
	helpPoint.x = helpPoint.y = 0;
	var _putSuccess;
	var _tryTimes = 0;
	do{
		_putSuccess = currentLibrary.addItemToDocument(helpPoint, _textureName);
		_tryTimes ++;
	}while(!_putSuccess && _tryTimes < 5);
	if(!_putSuccess){
		trace("内存不足导致放置贴图失败！请尝试重新导入。");
		return false;
	}
	
	_symbol = currentDom.selection[0];
	if(_symbol.symbolType != MOVIE_CLIP){
		_symbol.symbolType = MOVIE_CLIP;
	}
	var _subTextureXML = <{SUB_TEXTURE} {A_NAME} = {_textureName}/>;
	_subTextureXML[AT + A_PIVOT_X] = formatNumber(_symbol.x - _symbol.left);
	_subTextureXML[AT + A_PIVOT_Y] = formatNumber(_symbol.y - _symbol.top);
	_subTextureXML[AT + A_WIDTH] = Math.ceil(_symbol.width);
	_subTextureXML[AT + A_HEIGHT] = Math.ceil(_symbol.height);
	
	if(_isLast){
		_timeline.removeFrames(1, 1);
		if(currentItemBackup){
			currentLibrary.editItem(currentItemBackup.name);
			currentDom.getTimeline().currentFrame = currentFrameBackup;
		}
	}else{
		_timeline.currentFrame = 1;
	}
	return _subTextureXML.toXMLString();
}

Skeleton.packTextures = function(_textureAtlasXML){
	if(errorDOM()){
		return false;
	}
	
	if(!currentLibrary.itemExists(TEXTURE_SWF_ITEM)){
		return false;
	}
	_textureAtlasXML = XML(_textureAtlasXML).toXMLString();
	_textureAtlasXML = replaceString(_textureAtlasXML, "&lt;", "<");
	_textureAtlasXML = replaceString(_textureAtlasXML, "&gt;", ">");
	_textureAtlasXML = XML(_textureAtlasXML);
	
	var _subTextureXMLList = _textureAtlasXML[SUB_TEXTURE];
	
	var _textureItem = currentLibrary.items[currentLibrary.findItemIndex(TEXTURE_SWF_ITEM)];
	var _timeline = _textureItem.timeline;
	_timeline.currentFrame = 0;
	var _name;
	var _subTextureXML;
	for each(var _texture in _textureItem.timeline.layers[0].frames[0].elements){
		_subTextureXML = _subTextureXMLList.(@name == _texture.libraryItem.name)[0];
		if(_subTextureXML){
			if(_texture.scaleX != 1){
				_texture.scaleX = 1;
			}
			if(_texture.scaleY != 1){
				_texture.scaleY = 1;
			}
			if(_texture.skewX != 0){
				_texture.skewX = 0;
			}
			if(_texture.skewY != 0){
				_texture.skewY = 0;
			}
			_texture.x += Number(_subTextureXML[AT + A_X]) - _texture.left;
			_texture.y += Number(_subTextureXML[AT + A_Y]) - _texture.top;
		}
	}
	currentDom.selectAll();
	currentDom.selectNone();
	return true;
}

Skeleton.exportSWF = function(){
	if(errorDOM()){
		return "";
	}
	
	if(!currentLibrary.itemExists(TEXTURE_SWF_ITEM)){
		return "";
	}
	var _folderURL = fl.configURI;
	var _pathDelimiter;
	if(_folderURL.indexOf("/")>=0){
		_pathDelimiter = "/";
	}else if(_folderURL.indexOf("\\")>=0){
		_pathDelimiter = "\\";
	}else{
		return "";
	}
	_folderURL = _folderURL + "WindowSWF" + _pathDelimiter + SKELETON_PANEL;
	if(!FLfile.exists(_folderURL)){
		FLfile.createFolder(_folderURL);
	}
	var _swfURL = _folderURL + _pathDelimiter + TEXTURE_SWF;
	currentLibrary.items[currentLibrary.findItemIndex(TEXTURE_SWF_ITEM)].exportSWF(_swfURL);
	return _swfURL;
}

//Write armatureConnection data by armatureName
Skeleton.changeArmatureConnection = function(_armatureName, _data){
	if(errorDOM()){
		return false;
	}
	var _item = currentLibrary.items[currentLibrary.findItemIndex(_armatureName)];
	if(!_item){
		trace("cannot find " + _armatureName + " element，please make sure your fla file is synchronized！");
		return false;
	}
	_data = XML(_data).toXMLString();
	_data = replaceString(_data, "&lt;", "<");
	_data = replaceString(_data, "&gt;", ">");
	setArmatureConnection(_item, _data);
	return true;
}

Skeleton.changeMovement = function(_armatureName, _movementName, _data){
	if(errorDOM()){
		return false;
	}
	var _item = currentLibrary.items[currentLibrary.findItemIndex(_armatureName)];
	if(!_item){
		trace("cannot find " + _armatureName + " element，please make sure your fla file is synchronized！");
		return false;
	}
	
	_data = XML(_data).toXMLString();
	_data = replaceString(_data, "&lt;", "<");
	_data = replaceString(_data, "&gt;", ">");
	_data = XML(_data);
	delete _data[BONE].*;
	
	var _animationXML;
	if(_item.hasData(ANIMATION_DATA)){
		_animationXML = XML(_item.getData(ANIMATION_DATA));
	}else{
		_animationXML = <{ANIMATION}/>;
	}
	var _movementXML = _animationXML[MOVEMENT].(@name == _movementName)[0];
	if(_movementXML){
		_animationXML[MOVEMENT][_movementXML.childIndex()] = _data;
	}else{
		_animationXML.appendChild(_data);
	}
	_item.addData(ANIMATION_DATA, STRING, _animationXML.toXMLString());
	//Jsfl api Or Flash pro bug
	_item.symbolType = GRAPHIC;
	_item.symbolType = MOVIE_CLIP;
	return true;
}

})();