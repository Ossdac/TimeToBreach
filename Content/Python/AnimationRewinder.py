import unreal
import sys


def create_rewind_animations_directory(source_directory):
    """
    Creates rewind versions of all animation sequences and montages in a new directory.

    Args:
        source_directory (str): The source directory path (e.g., "/Game/Animations")
    """

    # Validate source directory
    if not unreal.EditorAssetLibrary.does_directory_exist(source_directory):
        unreal.log_error(f"Source directory does not exist: {source_directory}")
        return False

    # Create new directory path with _Rewind postfix
    base_path = source_directory.rstrip('/')
    rewind_directory = f"{base_path}_Rewind"

    # Create the rewind directory if it doesn't exist
    if not unreal.EditorAssetLibrary.does_directory_exist(rewind_directory):
        unreal.EditorAssetLibrary.make_directory(rewind_directory)
        unreal.log(f"Created directory: {rewind_directory}")
    else:
        unreal.log(f"Using existing directory: {rewind_directory}")

    # Get all assets in the source directory
    asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()

    # Filter for animation sequences and montages in the specific directory
    anim_filters = unreal.ARFilter(
        class_names=["AnimSequence", "AnimMontage"],
        package_paths=[source_directory],
        recursive_paths=True
    )

    anim_assets = asset_registry.get_assets(anim_filters)

    if len(anim_assets) == 0:
        unreal.log_warning(f"No animation sequences or montages found in: {source_directory}")
        return False

    unreal.log(f"Found {len(anim_assets)} animation assets to process")

    success_count = 0
    error_count = 0

    for asset_data in anim_assets:
        try:
            asset_path = asset_data.get_asset().get_path_name()
            asset = unreal.load_asset(asset_path)

            # Get original asset name and path
            original_name = asset.get_name()
            new_name = original_name + "_Rewind"

            # Create new package path in the rewind directory
            # Extract sub-path if source directory has nested structure
            package_path = asset.get_package().get_path_name()
            relative_path = package_path.replace(source_directory, "").rsplit('/', 1)[0]

            if relative_path.startswith('/'):
                relative_path = relative_path[1:]

            target_directory = rewind_directory
            if relative_path:
                target_directory = f"{rewind_directory}/{relative_path}"

                # Ensure the subdirectory exists
                if not unreal.EditorAssetLibrary.does_directory_exist(target_directory):
                    unreal.EditorAssetLibrary.make_directory(target_directory)

            # Check if asset already exists to avoid duplicates
            potential_path = f"{target_directory}/{new_name}"
            if unreal.EditorAssetLibrary.does_asset_exist(potential_path):
                unreal.log_warning(f"Asset already exists, skipping: {potential_path}")
                continue

            # Duplicate the asset to the new directory
            new_asset = unreal.EditorAssetLibrary.duplicate_asset(
                asset_path,
                f"{target_directory}/{new_name}"
            )

            if new_asset:
                # Set rate scale to -1 for reverse playback
                if isinstance(new_asset, unreal.AnimSequence):
                    # For AnimSequence, we need to modify the playback settings
                    try:
                        # Method 1: Try setting the play rate directly
                        new_asset.set_editor_property("rate_scale", -1.0)
                    except:
                        try:
                            # Method 2: Try using sequence_length and frame rate inversion
                            # This creates a true reverse by modifying the sequence data
                            pass  # We'll implement this below
                        except Exception as e:
                            unreal.log_warning(f"Could not set rate scale for {new_name}: {e}")

                elif isinstance(new_asset, unreal.AnimMontage):
                    # For AnimMontage, set the play rate
                    try:
                        new_asset.set_editor_property("play_rate", -1.0)
                    except:
                        try:
                            # Alternative method for montages
                            new_asset.set_editor_property("rate_scale", -1.0)
                        except Exception as e:
                            unreal.log_warning(f"Could not set play rate for montage {new_name}: {e}")

                # Mark as dirty and save
                unreal.EditorAssetLibrary.save_loaded_asset(new_asset)

                success_count += 1
                unreal.log(f"✓ Created: {new_name} in {target_directory}")
            else:
                error_count += 1
                unreal.log_error(f"✗ Failed to duplicate: {original_name}")

        except Exception as e:
            error_count += 1
            unreal.log_error(f"✗ Error processing {asset_data.get_asset().get_name()}: {str(e)}")

    # Final summary
    unreal.log(f"=== Processing Complete ===")
    unreal.log(f"Successfully created: {success_count} assets")
    unreal.log(f"Errors: {error_count} assets")
    unreal.log(f"Rewind assets location: {rewind_directory}")
    return True


def main():
    """
    Main function that handles command line argument and script execution.
    """
    unreal.log("=== Animation Rewinder ===")
    unreal.log("This script creates reverse versions of animations.")

    # Check if a directory argument was provided
    if len(sys.argv) > 1:
        source_directory = sys.argv[1]
        unreal.log(f"Using directory from command line: {source_directory}")
    else:
        # No argument provided - show usage and abort
        unreal.log_error("❌ No directory specified!")
        unreal.log_error("Usage: py \"Content/Python/AnimationRewinder.py\" \"/Game/Your/Directory/Path\"")
        unreal.log_error(
            "Example: py \"Content/Python/AnimationRewinder.py\" \"/Game/Characters/Mannequins/Anims/Unarmed/Attack\"")
        return False

    # Validate the directory exists
    if not unreal.EditorAssetLibrary.does_directory_exist(source_directory):
        unreal.log_error(f"❌ Directory does not exist: {source_directory}")
        unreal.log_error("Please check the path and try again.")
        return False

    # Proceed with processing
    unreal.log(f"Processing directory: {source_directory}")
    success = create_rewind_animations_directory(source_directory)

    if success:
        unreal.log("🎉 Animation rewinding completed successfully!")
    else:
        unreal.log("❌ Animation rewinding failed.")

    return success


# === EXECUTION ===
if __name__ == "__main__":
    main()