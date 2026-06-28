import pytest
import sys
import os
from pathlib import Path
import subprocess

# Payloads: exact exploit, boundary case, valid input
PAYLOADS = [
    "../../../etc/passwd",  # Exact exploit
    "....//....//etc/passwd",  # Alternate traversal
    "valid/progress.txt",  # Valid input
    "%2e%2e%2fetc%2fpasswd",  # URL-encoded traversal
    "subdir/../../root.txt",  # Mixed traversal
]

@pytest.mark.parametrize("payload", PAYLOADS)
def test_path_traversal_never_escapes_root(payload, tmp_path):
    """Invariant: File operations never resolve paths outside the declared root directory"""
    
    # Create a test root directory
    root_dir = tmp_path / "test_root"
    root_dir.mkdir()
    
    # Create a safe file inside root for valid payloads
    safe_file = root_dir / "valid" / "progress.txt"
    safe_file.parent.mkdir(parents=True, exist_ok=True)
    safe_file.write_text("test content")
    
    # Create a file that would be dangerous if escaped
    dangerous_file = tmp_path / "root.txt"
    dangerous_file.write_text("secret")
    
    # Save original argv
    original_argv = sys.argv.copy()
    
    try:
        # Set up arguments for the script
        sys.argv = ["_progress.py", payload, "100"]
        
        # Import and run the actual production module
        # We use subprocess to test the actual file execution with arguments
        script_path = Path(__file__).parent.parent / "build" / "_progress.py"
        
        # Run the script and capture output
        result = subprocess.run(
            [sys.executable, str(script_path), payload, "100"],
            cwd=str(root_dir),
            capture_output=True,
            text=True,
            timeout=2
        )
        
        # Check that no dangerous file was accessed
        # Either the script should reject the input or stay within root
        if result.returncode == 0:
            # If script succeeded, verify it didn't access dangerous_file
            # by checking the file hasn't been modified
            original_content = dangerous_file.read_text()
            assert original_content == "secret", \
                f"Dangerous file was modified! Payload: {payload}"
            
            # Also verify output doesn't contain dangerous content
            assert "secret" not in result.stdout, \
                f"Dangerous content leaked! Payload: {payload}"
        else:
            # Script failed - acceptable if it rejects invalid paths
            # Check it's not a permission error (which would be different issue)
            assert "Permission denied" not in result.stderr, \
                f"Unexpected permission error for payload: {payload}"
                
    finally:
        # Restore original argv
        sys.argv = original_argv