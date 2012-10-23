//
//  PHURLLoader+Automation.h
//  playhaven-sdk-ios
//
//  Created by Jesus Fernandez on 6/6/12.
//  Copyright (c) 2012 Playhaven. All rights reserved.
//

#import "PHURLLoader.h"

@interface PHURLLoader (Automation)
+(NSURL *)lastLaunchedURL;
+(void)setLastLaunchedURL:(NSURL *)url;
-(void)_launchURLForAutomation:(NSURL *)targetURL;
@end
