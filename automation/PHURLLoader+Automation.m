//
//  PHURLLoader+Automation.m
//  playhaven-sdk-ios
//
//  Created by Jesus Fernandez on 6/6/12.
//  Copyright (c) 2012 Playhaven. All rights reserved.
//

#import "PHURLLoader+Automation.h"

static NSURL *LastLaunchedURL;

@implementation PHURLLoader (Automation)
+(NSURL *)lastLaunchedURL{
    @synchronized(self){
        return LastLaunchedURL;
    }
}

+(void)setLastLaunchedURL:(NSURL *)url{
    @synchronized(self){
        [LastLaunchedURL release], LastLaunchedURL = [url copy];
    }
}

-(void)_launchURLForAutomation:(NSURL *)targetURL{
    //App switching interferes with automation testing
    //Instead, we pretend to launch the URL.
    NSLog(@"Pretending to launch URL: %@", targetURL);
    [PHURLLoader setLastLaunchedURL:targetURL];
}
@end
