//  
// Copyright (c) articy Software GmbH & Co. KG. All rights reserved.  
//

#include "Customizations/ArticyEditorCustomizationManager.h"
#include "ArticyObject.h"

FArticyEditorCustomizationManager::FArticyEditorCustomizationManager()
{
}

IArticyRefWidgetCustomizationFactory* FArticyEditorCustomizationManager::RegisterArticyRefWidgetCustomizationFactory(FOnCreateArticyRefWidgetCustomizationFactory GetCustomizationDelegate)
{
	ArticyRefWidgetCustomizationFactories.Add(GetCustomizationDelegate.Execute());
	return ArticyRefWidgetCustomizationFactories.Last().Get();
}

void FArticyEditorCustomizationManager::UnregisterArticyRefWidgetCustomizationFactory(const IArticyRefWidgetCustomizationFactory* Factory)
{
	const int32 RemovedElements = ArticyRefWidgetCustomizationFactories.RemoveAll([=](const TSharedPtr<IArticyRefWidgetCustomizationFactory> FactoryPtr)
	{
		return Factory == FactoryPtr.Get();
	});

	ensureMsgf(RemovedElements != 0, TEXT("Failed removing factory. It was either not registered or removed already."));
}

void FArticyEditorCustomizationManager::CreateArticyRefWidgetCustomizations(const UArticyObject* ArticyObject, TArray<TSharedPtr<IArticyRefWidgetCustomization>>& OutCustomizations)
{
	if(ArticyObject == nullptr)
	{
		return;
	}
	
	for (const TSharedPtr<IArticyRefWidgetCustomizationFactory>& Entry : ArticyRefWidgetCustomizationFactories)
	{
		if(Entry->SupportsType(ArticyObject))
		{
			OutCustomizations.Add(Entry->CreateCustomization());
		}
	}
}
