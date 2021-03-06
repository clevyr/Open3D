# CI workflows

## A. Documentation deployment

### Directory structure

-   `.github/workflows/documentation.yml`: Github Actions workflow file to
    create and deploy documentation. Documentation is created for every branch
    as a CI test, but deployed only for `master`.

-   `util/make_documentation.sh`: Called by GitHub Actions to buld documentation.

-   `unpack_docs.sh`: Called by the documentation server to deploy the docs into
    the website.

### Setting up the docs deployment

#### Step 1: Google Cloud

1.  Setup `gcloud`

   -   [Install `gcloud`](https://cloud.google.com/sdk/install)
   -   `gcloud init` and login with admin's google account on the web

2.  Create storage bucket

   ```bash
   # Create with uniform bucket-level access: `-b`
   # https://cloud.google.com/storage/docs/creating-buckets#storage-create-bucket-gsutil
   # gsutil mb -p [PROJECT_ID] -c [STORAGE_CLASS] -l [BUCKET_LOCATION] -b on gs://[BUCKET_NAME]/
   gsutil mb -p isl-buckets -c STANDARD -l US -b on gs://open3d-docs/

   # Grant public read permission
   # The current user must have appropriate permission, you may do this in the web interface
   gsutil acl ch -u AllUsers:R gs://open3d-docs/

   # Set object life cycle
   # https://cloud.google.com/storage/docs/managing-lifecycles#delete_an_object
   gsutil lifecycle set gcs.lifecycle.json gs://open3d-docs/
   ```

   Objects will be stored in the bucket for one week. Currently, the
   documentation server fetches the latest docs from `master` branch every hour.
   If the documentation server fails to fetch the docs matching the `master`
   commit id, the last successfully fetched docs will be displayed.

3.  Create service account

   ```bash
   gcloud iam service-accounts create open3d-ci-sa \
       --description="Service account for Open3D CI" \
       --display-name="open3d-ci-sa"
   ```

4.  Grant `objectAdmin` to the service account

   ```bash
   gsutil iam ch \
       serviceAccount:open3d-ci-sa@isl-buckets.iam.gserviceaccount.com:objectAdmin \
       gs://open3d-docs
   ```

5.  Create key for service account

   ```bash
   gcloud iam service-accounts keys create ~/open3d-ci-sa-key.json \
     --iam-account open3d-ci-sa@isl-buckets.iam.gserviceaccount.com
   ```

   Now `~/open3d-ci-sa-key.json` should have been created.

#### Step 2: GitHub

1.  Encode the private key json file with `base64 ~/open3d-ci-sa-key.json` and
    add the output text to the
    [GitHub repository secrets](https://github.com/intel-isl/Open3D/settings/secrets)
    with name `GCE_SA_KEY_DOCS_CI`

2.  Also add secret `GCE_DOCS_PROJECT: isl-buckets`

## B. Google compute engine setup for GPU CI

### CI Procedure

The GCE CI workflow `.github/workflows/gce-ubuntu-docker.yml` performs these steps:

-   Clone the repository

-   Build docker image, starting with a an NVIDIA base devel image with CUDA and
    cuDNN.

-   Push image to Google container registry.

-   On Google Compute Engine (GCE), in parallel (up to GPU quota limit - currently
    4):
  -   Create a new VM instance with a custom OS image

  -   Run docker image on GCE (Google Compute Engine) with environment variables
      set for specific build config.

  -   The docker image entrypoint is the `run-ci.sh` script: build, install, run
      tests and uninstall.

  -   Delete the VM instance.

A separate VM instance is created for each commit and build option. The VM
instances are named according to the commit hash and build config ID used. We
cycle through 13 different US GCE zones if VM creation fails in the first zone,
either due to lack of resources or GPU quota exhaustion.

### Setup

#### Step 1: Google Cloud: Create service account and key

1.  Create service account

    ```bash
    gcloud iam service-accounts create open3d-ci-sa \
        --description="Service account for Open3D CI" \
        --display-name="open3d-ci-sa"    \
        --project open3d-dev
    ```

2.  Grant `Compute Instance Admin (beta)` to the service account

    ```bash
    gcloud projects add-iam-policy-binding open3d-dev \
        --member=serviceAccount:open3d-ci-sa-gpu@open3d-dev.iam.gserviceaccount.com \
        --role=roles/compute.instanceAdmin \
        --project open3d-dev
    ```

3.  Create key for service account

    ```bash
    gcloud iam service-accounts keys create ~/open3d-ci-sa-key.json \
        --iam-account open3d-ci-sa@open3d-dev.iam.gserviceaccount.com \
        --project open3d-dev
    ```

    Now `~/open3d-ci-sa-key.json` should have been created.

#### Step 2: Google Cloud: Create custom VM image

```bash
./util/docker/open3d-gpu/scripts/gce-ubuntu-docker-run.sh create-base-vm-image
```

The custom VM image has NVIDIA drivers, `nvidia-container-toolkit` and `docker`
installed. It contains today's date in the name and the image family is set to
`ubuntu-os-docker-gpu-2004-lts`. The latest image from this family is
used for running CI.

#### Step 3: GitHub

1.  Encode the private key json file with `base64 ~/open3d-ci-sa-key.json` and
    add the output text to the
    [GitHub repository secrets](https://github.com/intel-isl/Open3D/settings/secrets)
    with name `GCE_SA_KEY_GPU_CI`

2.  Also add secret `GCE_PROJECT: open3d-dev`
