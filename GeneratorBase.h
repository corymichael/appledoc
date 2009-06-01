//
//  GeneratorBase.h
//  appledoc
//
//  Created by Tomaz Kragelj on 28.5.09.
//  Copyright (C) 2009, Tomaz Kragelj. All rights reserved.
//

#import <Foundation/Foundation.h>

enum TKGeneratorSectionItemTypes
{
	kTKSectionItemInherits,
	kTKSectionItemConforms,
	kTKSectionItemDeclared,
};

enum TKGeneratorMemberTypes
{
	kTKMemberTypeClass,
	kTKMemberTypeInstance,
	kTKMemberTypeProperty,
};

enum TKGeneratorPrototypeTypes
{
	kTKMemberPrototypeValue,
	kTKMemberPrototypeParameter,
};

enum TKGeneratorMemberSectionTypes
{
	kTKMemberSectionParameters,
	kTKMemberSectionExceptions,
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/** Defines the basics for an output generator.

￼￼Output generators are objects that generate final output files from the intermediate
(cleaned) XML. Each type of supported output is implemented by a concrete subclass.
This class should be treated as abstract base class. It provides the stubs for the
output generation as well as several helper methods that the subclasses can use to
make their job easier.
 
There are two options for generating output. The first is to use the default stubs. This
means that the subclass should leave the layout and order of creation to the base class
and should override several methods which are sent during the creation, depending the
object that is being generated. The methods which fall into this category are easily
identified by their @c append prefix. This is the most common and also the simplest way 
of getting the job done. However it limits the order in which output elements are generated. 
If the subclass requires more control, it can also override @c outputDataForObject() 
method and handle the data in a completely custom way (@c outputDataForObject() 
message is sent from @c generateOutputForObject:() which sets the class 
properties with the object data, so the subclass can use these to make it's life easier.
 
The class is designed so that the same instance can be re-used for generation of several
objects by simply sending the instance @c generateOutputForObject:() message 
with the object data.
*/
@interface GeneratorBase : NSObject
{
	NSDictionary* objectData;
	NSString* lastUpdated;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Generation entry points
//////////////////////////////////////////////////////////////////////////////////////////

/** Generates the output￼ data from the given object data.

This is the main message that starts the whole generation for the given object. It copies 
the values from the given data to the properties and then sends the receiver 
@c outputDataForObject() message that triggers the object data parsing and in
turn sends the receiver several messages that can be used to convert the data

@param data ￼￼￼￼￼￼An @c NSDictionary that describes the object for which output is generated.
@exception ￼￼￼￼￼NSException Thrown if generation fails or the given @c data is @c nil.
@see outputDataForObject
*/
- (NSData*) generateOutputForObject:(NSDictionary*) data;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Subclasses handling for objects output generation
//////////////////////////////////////////////////////////////////////////////////////////

/** Generates the output data from the data contained in the class properties.￼

This message is sent from @c generateOutputForObject:() after the passed object data is 
stored in the class properties. The concrete subclasses that require full control over the 
generated data, can override this method and return the desired output. If overriden, the 
subclass can get the XML document through the @c objectMarkup property.
 
By default, this will send several higher level messages which can be overriden instead.
The messages are sent in the following order:
- @c appendObjectHeaderToData:()
 
- @c appendObjectInfoHeaderToData:() @a *
- @c appendObjectInfoItemToData:fromItem:index:type:() @a **
- @c appendObjectInfoFooterToData:() @a *
 
- @c appendObjectOverviewToData:fromItem:() @a *

- @c appendObjectTasksHeaderToData:() @a *
- @c appendObjectTaskHeaderToData:fromItem:index:() @a **
- @c appendObjectTaskMemberToData:fromItem:index:() @a **
- @c appendObjectTaskFooterToData:fromItem:index:() @a **
- @c appendObjectTasksFooterToData:() @a *
 
- @c appendObjectMembersHeaderToData:() @a *
- @c appendObjectMemberGroupHeaderToData:type:() @a **
- @c appendObjectMemberToData:fromItem:index:() @a **
- @c appendObjectMemberGroupFooterToData:() @a **
- @c appendObjectMembersFooterToData:() @a *
 
- @c appendObjectFooterToData:()
 
Note that only a subset of above messages may be sent for a particular object, depending
on the object data. Messages marked with @a * are optional, while messages marked with 
@a ** may additionaly be sent multiple times, for each corresponding item once.

@return ￼￼￼￼Returns an autoreleased @c NSData containing generated output.
@exception ￼￼￼￼￼NSException Thrown if generation fails.
@see generateOutputForObject:
*/
- (NSData*) outputDataForObject;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Subclass file header and footer handling
//////////////////////////////////////////////////////////////////////////////////////////

/** Appends any header text before the actual generation starts.￼

The message is sent from the @c outputDataForObject() as the first message. It gives 
subclasses a chance to append data to the output before the actual output generation starts. 
After this message is sent, the rest of the messages are followed and as the last one,
@c appendObjectFooterToData:() is sent.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectFooterToData:
*/
- (void) appendObjectHeaderToData:(NSMutableData*) data;

/** Appends any footer text after output generation ends.￼

The message is sent from the @c outputDataForObject() as the last message. It 
gives subclasses a chance to append data to the output after the rest of the output is 
generated. This is ussually the place to "close" open tags or similar.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectHeaderToData:
*/
- (void) appendObjectFooterToData:(NSMutableData*) data;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Subclass object info handling
//////////////////////////////////////////////////////////////////////////////////////////

/** Appends any object info header text before the actual generation starts.￼

The message is sent from the @c outputDataForObject() just before object info 
items are generated. It gives subclasses a chance to append data to the output before the 
generation for secion items starts. After this message is sent, at least one 
@c appendObjectInfoItemToData:fromItem:index:() messages is sent and then 
@c appendObjectInfoHeaderToData:() is sent at the end.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectInfoItemToData:fromItems:index:type:
@see appendObjectInfoFooterToData:
*/
- (void) appendObjectInfoHeaderToData:(NSMutableData*) data;

/** Appends any object info footer text after the object info items generation ends.￼

The message is sent from the @c outputDataForObject() as the first message. It 
gives subclasses a chance to append data to the output before the generation for secion 
items starts. After this message is sent, at least one @c appendObjectInfoItemToData:fromItem:index:() 
messages is sent and then @c appendObjectInfoHeaderToData:() is sent at the end.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectInfoHeaderToData:
@see appendObjectInfoItemToData:fromItems:index:type:
*/
- (void) appendObjectInfoFooterToData:(NSMutableData*) data;

/** ￼Appends the given object info item data.￼

This message is sent from @c outputDataForObject() for each applicable object info 
item type. The subclass should append the data for the given item. The subclass can get 
more information about the object info item by investigating the given array which 
contains objects that can be used to query additional data for individual item.
 
The type identifies the type of the info item and can be one of the following:
- @c kTKInfoItemInherits: The @c nodes contain inherit from information. Only one node
	is in the list in most (all) cases.
- @c kTKInfoItemConforms: The @c nodes contain conforms to information. The nodes list may
	contain one or more protocols to which the object conforms.
- @c kTKInfoItemDeclared: The @c nodex contain declared in information. Only one node
	is in the list in most (all) cases.
 
@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@param items ￼￼￼￼￼￼The array of info items instances describing individual items.
@param index ￼￼￼￼￼￼Zero based index of the item within the info object info.
@param type ￼￼￼￼￼￼The type of the item.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectInfoHeaderToData:
@see appendObjectInfoFooterToData:
*/
- (void) appendObjectInfoItemToData:(NSMutableData*) data
						  fromItems:(NSArray*) items
							  index:(int) index
							   type:(int) type;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Subclass object overview handling
//////////////////////////////////////////////////////////////////////////////////////////

/** Appends￼ object overview description.

This message is sent from @c outputDataForObject() if the object has brief and or 
detailed documentation assigned. It gives subclasses a chance to append object overview 
from the gathered documentation. The given @c item contains brief and detailed object
description and can be treated as any other description item.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@param item ￼￼￼￼￼￼The item that contains the brief and detailed description.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
*/
- (void) appendObjectOverviewToData:(NSMutableData*) data 
						   fromItem:(id) item;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Subclass object tasks handling
//////////////////////////////////////////////////////////////////////////////////////////

/** Appends any tasks header before the actual tasks generation starts.￼

The message is sent from the @c outputDataForObject() if the object has at least 
one task defined. It gives subclasses a chance to append data to the output before any 
individual task handling is started. After this message is sent, each individual 
task is handled and when all tasks are done, @c appendObjectTasksFooterToData:() is sent.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectTaskHeaderToData:fromItem:index:
@see appendObjectTaskFooterToData:fromItem:index:
@see appendObjectTasksFooterToData:
*/
- (void) appendObjectTasksHeaderToData:(NSMutableData*) data;

/** Appends any tasks footer after sections generation ends.￼

The message is sent from the @c outputDataForObject() after all tasks have 
been processed. It gives subclasses a chance to append data to the output at that point. 
This is ussually the place to "close" tasks open tags or similar.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectTasksHeaderToData:
@see appendObjectTaskHeaderToData:fromItem:index:
@see appendObjectTaskMemberToData:fromItem:index:
@see appendObjectTaskFooterToData:fromItem:index:
*/
- (void) appendObjectTasksFooterToData:(NSMutableData*) data;

/** Appends an individual task header before the task members generation starts.￼

The message is sent from the @c outputDataForObject() for each task which has 
at least one member. It gives subclasses a chance to append data to the output before 
member handling for the given task starts.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@param item ￼￼￼￼￼￼The item that contains the task description.
@param index Zero based index of the task.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectTasksHeaderToData:
@see appendObjectTaskMemberToData:fromItem:index:
@see appendObjectTaskFooterToData:fromItem:index:
@see appendObjectTasksFooterToData:
*/
- (void) appendObjectTaskHeaderToData:(NSMutableData*) data
							 fromItem:(id) item
								index:(int) index;

/** Appends an individual task footer after the task members generation ends.￼

The message is sent from the @c outputDataForObject() after all task members 
have been processed. It gives subclasses a chance to append data to the output at that 
point. This is ussually the place to "close" tasks open tags or similar.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@param item ￼￼￼￼￼￼The item that contains the task description.
@param index Zero based index of the task.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectTasksHeaderToData:
@see appendObjectTaskHeaderToData:fromItem:index:
@see appendObjectTaskMemberToData:fromItem:index:
@see appendObjectTasksFooterToData:
*/
- (void) appendObjectTaskFooterToData:(NSMutableData*) data
							 fromItem:(id) item
								index:(int) index;

/** Appends a task member data.￼

This message is sent from the @c outputDataForObject() for each task member (class or
instance method or property. Subclasses should append any desired data for the given
member.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@param item ￼￼￼￼￼￼The item that contains the task member description.
@param index ￼￼￼￼￼￼Zero based index of the member within the task.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectTasksHeaderToData:
@see appendObjectTaskHeaderToData:fromItem:index:
@see appendObjectTaskFooterToData:fromItem:index:
@see appendObjectTasksFooterToData:
*/
- (void) appendObjectTaskMemberToData:(NSMutableData*) data
							 fromItem:(id) item
								index:(int) index;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Subclass member groups handling
//////////////////////////////////////////////////////////////////////////////////////////

/** Appends any main members documentation header before the actual member generation starts.￼

The message is sent from the @c outputDataForObject() if the object has at least 
one main member group defined. It gives subclasses a chance to append data to the output 
before  any individual member handling is started. After this message is sent, each individual 
member group is handled and when all members are done, @c appendObjectMembersFooterToData:() is 
sent.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectMembersFooterToData:
@see appendObjectMemberGroupHeaderToData:type:
@see appendObjectMemberGroupFooterToData:type:
@see appendObjectMemberToData:fromItem:index:
*/
- (void) appendObjectMembersHeaderToData:(NSMutableData*) data;

/** Appends any main members documentation footer after members generation ends.￼

The message is sent from the @c outputDataForObject() after all member groups 
have been processed. It gives subclasses a chance to append data to the output at that point. 
This is ussually the place to "close" sections open tags or similar.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectMembersHeaderToData:
@see appendObjectMemberGroupHeaderToData:type:
@see appendObjectMemberGroupFooterToData:type:
@see appendObjectMemberToData:fromItem:index:
*/
- (void) appendObjectMembersFooterToData:(NSMutableData*) data;

/** ￼Appends member group header data.
 
This message is sent from @c outputDataForObject() for each main member 
documentation group. The group is specified with the @c type parameter and can be one of 
the following:￼
- @c kTKMemberTypeClass: The group describes class members.
- @c kTKMemberTypeInstance: The group describes instance members.
- @c kTKMemberTypeProperty: The group describes properties.

Subclasses should append any desired data for the given group type. The message is only
sent if at least one member of the given type is documented for the object. After this
message one or more @c appendObjectMemberToData:fromItem:index:() messages are sent, one for
each documented member of the given group and after all members output is generated,
@c appendObjectMemberGroupHeaderToData:type:() is sent.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@param type ￼￼￼￼￼￼The type of the group that is being described.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectMembersHeaderToData:
@see appendObjectMembersFooterToData:
@see appendObjectMemberToData:fromItem:index:
@see appendObjectMemberGroupFooterToData:type:
*/
- (void) appendObjectMemberGroupHeaderToData:(NSMutableData*) data 
										type:(int) type;

/** Appends member group footer data.￼

This message is sent from @c outputDataForObject() for each main member 
documentation group. The group is specified by the @c type parameter and can be one of 
the following:
- @c kTKMemberTypeClass: The group describes class members.
- @c kTKMemberTypeInstance: The group describes instance members.
- @c kTKMemberTypeProperty: The group describes properties.
 
Subclasses should append any desired data for the given group type. In most cases this
is the place to close any open tags or similar. The message is only sent if the
corresponding @c appendObjectMemberGroupHeaderToData:type:() was sent.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@param type ￼￼￼￼￼￼The type of the group that is being described.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectMembersHeaderToData:
@see appendObjectMembersFooterToData:
@see appendObjectMemberGroupHeaderToData:type:
@see appendObjectMemberToData:fromItem:index:
*/
- (void) appendObjectMemberGroupFooterToData:(NSMutableData*) data 
										type:(int) type;

/** Appends individual member full documentation data.￼

This message is sent from @c outputDataForObject() for each documented member. 
Subclasses should output the full documentation for the given member.

@param data ￼￼￼￼￼￼The data to append to. This is guaranteed to be non @c null.
@param item ￼￼￼￼￼￼The item that describes the member data.
@param index ￼￼￼￼￼￼Zero based index of the member within the group.
@exception ￼￼￼￼￼NSException Thrown if appending fails.
@see outputDataForObject
@see appendObjectMembersHeaderToData:
@see appendObjectMembersFooterToData:
@see appendObjectMemberGroupHeaderToData:type:
@see appendObjectMemberGroupFooterToData:type:
*/
- (void) appendObjectMemberToData:(NSMutableData*) data 
						 fromItem:(id) item 
							index:(int) index;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Properties
//////////////////////////////////////////////////////////////////////////////////////////

/** Sets or returns the last updated date.￼

Clients should set this value prior to sending @c generateOutputForObject:() 
message. If the value is non @c nil and is not an empty string, the value can be used by 
the concrete generators to indicate the time of the last update.
*/
@property(copy) NSString* lastUpdated;

@end
